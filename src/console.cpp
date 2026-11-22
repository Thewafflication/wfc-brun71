#include "wfc/brun71/console.hpp"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <algorithm>
#include <vector>

#include "wfc/brun71/cp437.hpp"

namespace wfc::brun71 {
namespace {

HANDLE handle(void* value) {
    return static_cast<HANDLE>(value);
}

// The cursor of a cell 8 scan lines high: the share of the lines from start
// to stop, as the console takes it (1-100 percent).
DWORD cursor_size(int start, int stop) {
    const int lines = stop >= start ? stop - start + 1 : 8;
    return static_cast<DWORD>(std::clamp(lines * 100 / 8, 1, 100));
}

}  // namespace

std::unique_ptr<Console> Console::open(bool activate) {
    HANDLE previous = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info{};
    if (previous == INVALID_HANDLE_VALUE || previous == nullptr ||
        !GetConsoleScreenBufferInfo(previous, &info)) {
        previous = nullptr;
    }
    HANDLE buffer = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
    if (buffer == INVALID_HANDLE_VALUE) {
        return nullptr;
    }
    if (activate && !SetConsoleActiveScreenBuffer(buffer)) {
        CloseHandle(buffer);
        return nullptr;
    }
    HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    if (input == INVALID_HANDLE_VALUE || input == nullptr ||
        !GetConsoleMode(input, &mode)) {
        input = nullptr;
    }
    return std::unique_ptr<Console>(
        new Console(buffer, activate ? previous : nullptr, input));
}

Console::Console(void* buffer, void* previous, void* input)
    : buffer_(buffer), previous_(previous), input_(input) {}

Console::~Console() {
    if (mouse_) {
        SetConsoleMode(handle(input_), input_mode_);
    }
    if (previous_ != nullptr) {
        SetConsoleActiveScreenBuffer(handle(previous_));
    }
    CloseHandle(handle(buffer_));
}

void Console::present(const TextScreen& screen) {
    const int columns = screen.columns();
    const int rows = screen.rows();
    HANDLE out = handle(buffer_);
    if (columns != columns_ || rows != rows_) {
        // The window may not be larger than the buffer: shrink it first.
        CONSOLE_SCREEN_BUFFER_INFO info{};
        if (GetConsoleScreenBufferInfo(out, &info)) {
            const SMALL_RECT shrunk{
                0, 0,
                static_cast<SHORT>(
                    std::min<int>(
                        columns, info.srWindow.Right - info.srWindow.Left + 1) -
                    1),
                static_cast<SHORT>(
                    std::min<int>(
                        rows, info.srWindow.Bottom - info.srWindow.Top + 1) -
                    1)};
            SetConsoleWindowInfo(out, TRUE, &shrunk);
        }
        SetConsoleScreenBufferSize(
            out, COORD{static_cast<SHORT>(columns), static_cast<SHORT>(rows)});
        const SMALL_RECT full{0, 0, static_cast<SHORT>(columns - 1),
                              static_cast<SHORT>(rows - 1)};
        SetConsoleWindowInfo(out, TRUE, &full);
        columns_ = columns;
        rows_ = rows;
    }

    std::vector<CHAR_INFO> cells(screen.cells().size());
    for (std::size_t i = 0; i < cells.size(); ++i) {
        const Cell& cell = screen.cells()[i];
        const char32_t shown = cp437_to_unicode(cell.character);
        cells[i].Char.UnicodeChar = static_cast<WCHAR>(shown);
        cells[i].Attributes = static_cast<WORD>(cell.attribute & 0x7F);
    }
    SMALL_RECT region{0, 0, static_cast<SHORT>(columns - 1),
                      static_cast<SHORT>(rows - 1)};
    WriteConsoleOutputW(
        out, cells.data(),
        COORD{static_cast<SHORT>(columns), static_cast<SHORT>(rows)},
        COORD{0, 0}, &region);

    SetConsoleCursorPosition(out,
                             COORD{static_cast<SHORT>(screen.pos() - 1),
                                   static_cast<SHORT>(screen.csrlin() - 1)});
    const CONSOLE_CURSOR_INFO cursor{
        cursor_size(screen.cursor_start(), screen.cursor_stop()),
        screen.cursor_visible() ? TRUE : FALSE};
    SetConsoleCursorInfo(out, &cursor);
}

void Console::enable_mouse() {
    DWORD mode = 0;
    if (mouse_ || input_ == nullptr || !GetConsoleMode(handle(input_), &mode)) {
        return;
    }
    input_mode_ = mode;
    mode |= ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    mode &= ~static_cast<DWORD>(ENABLE_QUICK_EDIT_MODE);
    mouse_ = SetConsoleMode(handle(input_), mode) != FALSE;
}

int Console::poll(KeyBuffer& buffer, std::deque<MouseEvent>* mouse) {
    if (input_ == nullptr) {
        return 0;
    }
    int added = 0;
    for (;;) {
        DWORD waiting = 0;
        if (!GetNumberOfConsoleInputEvents(handle(input_), &waiting) ||
            waiting == 0) {
            break;
        }
        INPUT_RECORD record{};
        DWORD read = 0;
        if (!ReadConsoleInputW(handle(input_), &record, 1, &read) ||
            read == 0) {
            break;
        }
        if (record.EventType == MOUSE_EVENT && mouse != nullptr) {
            const MOUSE_EVENT_RECORD& event = record.Event.MouseEvent;
            const DWORD held = event.dwButtonState;
            const int buttons =
                ((held & FROM_LEFT_1ST_BUTTON_PRESSED) != 0 ? kLeftButton : 0) |
                ((held & RIGHTMOST_BUTTON_PRESSED) != 0 ? kRightButton : 0) |
                ((held & FROM_LEFT_2ND_BUTTON_PRESSED) != 0 ? kMiddleButton
                                                            : 0);
            const DWORD flags = event.dwEventFlags;
            if ((flags & (MOUSE_WHEELED | MOUSE_HWHEELED)) != 0) {
                continue;
            }
            const DWORD state = event.dwControlKeyState;
            for (MouseEvent change : mouse_changes(
                     buttons_, buttons, (flags & MOUSE_MOVED) != 0,
                     (flags & DOUBLE_CLICK) != 0, event.dwMousePosition.Y + 1,
                     event.dwMousePosition.X + 1)) {
                change.shift = (state & SHIFT_PRESSED) != 0;
                change.control =
                    (state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0;
                change.alt =
                    (state & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) != 0;
                mouse->push_back(change);
            }
            buttons_ = buttons;
            continue;
        }
        if (record.EventType != KEY_EVENT || !record.Event.KeyEvent.bKeyDown) {
            continue;
        }
        const KEY_EVENT_RECORD& event = record.Event.KeyEvent;
        const DWORD state = event.dwControlKeyState;
        KeyPress press;
        press.virtual_key = event.wVirtualKeyCode;
        press.character = event.uChar.UnicodeChar;
        press.shift = (state & SHIFT_PRESSED) != 0;
        press.control = (state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0;
        press.alt = (state & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) != 0;
        const auto key = translate(press);
        if (!key) {
            continue;
        }
        for (WORD i = 0; i < std::max<WORD>(event.wRepeatCount, 1); ++i) {
            if (buffer.push(*key)) {
                ++added;
            } else {
                MessageBeep(MB_OK);
            }
        }
    }
    return added;
}

bool Console::wait(int milliseconds) {
    if (input_ == nullptr) {
        Sleep(static_cast<DWORD>(std::max(milliseconds, 0)));
        return false;
    }
    return WaitForSingleObject(handle(input_),
                               static_cast<DWORD>(std::max(milliseconds, 0))) ==
           WAIT_OBJECT_0;
}

Console::Shown Console::shown(int row, int column) const {
    CHAR_INFO cell{};
    SMALL_RECT region{
        static_cast<SHORT>(column - 1), static_cast<SHORT>(row - 1),
        static_cast<SHORT>(column - 1), static_cast<SHORT>(row - 1)};
    Shown result;
    if (ReadConsoleOutputW(handle(buffer_), &cell, COORD{1, 1}, COORD{0, 0},
                           &region)) {
        result.character = cell.Char.UnicodeChar;
        result.attribute = cell.Attributes;
    }
    return result;
}

Console::Cursor Console::cursor() const {
    Cursor result;
    CONSOLE_SCREEN_BUFFER_INFO info{};
    if (GetConsoleScreenBufferInfo(handle(buffer_), &info)) {
        result.row = info.dwCursorPosition.Y + 1;
        result.column = info.dwCursorPosition.X + 1;
    }
    CONSOLE_CURSOR_INFO cursor{};
    if (GetConsoleCursorInfo(handle(buffer_), &cursor)) {
        result.visible = cursor.bVisible != FALSE;
        result.size = static_cast<int>(cursor.dwSize);
    }
    return result;
}

}  // namespace wfc::brun71
