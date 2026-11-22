// The soft-key line (keyline.hpp).
#include "wfc/brun71/keyline.hpp"

#include <cstddef>
#include <cstdint>

#include "wfc/brun71/error.hpp"

namespace wfc::brun71 {
namespace {

constexpr std::uint8_t kReverse = 0x70;  // black on white
constexpr int kColumnsPerKey = 8;
constexpr std::size_t kShown = 6;  // characters of a text on the line

void clear_line(TextScreen& screen) {
    const int row = screen.rows();
    for (int column = 1; column <= screen.columns(); ++column) {
        screen.put(row, column, Cell{32, screen.attribute()});
    }
}

}  // namespace

void SoftKeys::assign(const int key, const std::string_view text,
                      TextScreen& screen) {
    if (key < 1 || key > kKeys) {
        throw BasicError(kIllegalFunctionCall);
    }
    texts_[static_cast<std::size_t>(key - 1)] =
        std::string(text.substr(0, kLimit));
    redraw(screen);
}

void SoftKeys::show(TextScreen& screen) {
    shown_ = true;
    redraw(screen);
}

void SoftKeys::hide(TextScreen& screen) {
    if (shown_) {
        shown_ = false;
        clear_line(screen);
    }
}

void SoftKeys::redraw(TextScreen& screen) const {
    if (!shown_) {
        return;
    }
    clear_line(screen);
    const int row = screen.rows();
    const int keys = screen.columns() / kColumnsPerKey;
    for (int key = 1; key <= keys && key <= 10; ++key) {
        int column = (key - 1) * kColumnsPerKey + 1;
        const std::string number = std::to_string(key);
        for (const char digit : number) {
            screen.put(row, column++,
                       Cell{static_cast<std::uint8_t>(digit), 7});
        }
        const std::string& text = texts_[static_cast<std::size_t>(key - 1)];
        // Key 10 has a two-digit number and one character less.
        const std::size_t room = kShown + 1U - number.size();
        for (std::size_t i = 0; i < room; ++i) {
            auto character =
                static_cast<std::uint8_t>(i < text.size() ? text[i] : ' ');
            if (character < 32) {
                character = 32;
            }
            screen.put(row, column++, Cell{character, kReverse});
        }
    }
}

std::array<std::string, SoftKeys::kKeys> SoftKeys::list() const {
    std::array<std::string, kKeys> lines;
    for (int key = 1; key <= kKeys; ++key) {
        std::string line = "F" + std::to_string(key) + " ";
        for (const char c : texts_[static_cast<std::size_t>(key - 1)]) {
            line.push_back(static_cast<unsigned char>(c) < 32 ? ' ' : c);
        }
        lines[static_cast<std::size_t>(key - 1)] = line;
    }
    return lines;
}

}  // namespace wfc::brun71
