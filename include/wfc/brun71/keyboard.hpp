// The keyboard of BASIC PDS 7.1: the keys a program reads with INKEY$ and
// the type-ahead buffer they wait in.
//
// A key is either a character (a code page 437 byte; Enter is 13, Esc 27,
// Ctrl+A to Ctrl+Z are 1-26) or an extended key, which INKEY$ returns as two
// characters, CHR$(0) and the BIOS scan code: 72 Up, 80 Down, 75 Left,
// 77 Right, 59-68 F1-F10, 133-134 F11-F12, with their Shift, Ctrl and Alt
// codes. The grey cursor keys give the same codes as the keypad ones.
#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <optional>
#include <string>

namespace wfc::brun71 {

struct Key {
    // The character, or 0 for an extended key.
    std::uint8_t character = 0;
    // The BIOS scan code of an extended key (0 for a character).
    std::uint8_t code = 0;

    bool extended() const noexcept { return character == 0; }
    bool operator==(const Key&) const = default;
};

// The string INKEY$ returns for the key: one character, or CHR$(0) and the
// scan code.
std::string inkey_string(Key key);

// A key pressed on a Windows keyboard, as the console reports it.
struct KeyPress {
    // The virtual-key code (VK_*).
    std::uint16_t virtual_key = 0;
    // The Unicode character the key typed, or 0.
    char32_t character = 0;
    bool shift = false;
    bool control = false;
    bool alt = false;
};

// The key BASIC sees for a key press, or none for a key BASIC does not
// report on its own (Shift, Ctrl, Alt, Caps Lock, and characters outside code
// page 437).
std::optional<Key> translate(const KeyPress& press);

// The type-ahead buffer: keys wait here until the program reads them. It
// holds 15 keys, as the PC BIOS buffer does; a key pressed when it is full
// is lost (a host beeps).
class KeyBuffer {
public:
    static constexpr std::size_t kCapacity = 15;

    // Adds a key; false when the buffer was full and the key is lost.
    bool push(Key key);
    // INKEY$: the next key's string, or "" when none is waiting.
    std::string inkey();
    std::optional<Key> pop();
    std::size_t size() const noexcept { return keys_.size(); }
    bool empty() const noexcept { return keys_.empty(); }
    void clear() noexcept { keys_.clear(); }

private:
    std::deque<Key> keys_;
};

}  // namespace wfc::brun71
