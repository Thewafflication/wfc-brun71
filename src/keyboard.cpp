#include "wfc/brun71/keyboard.hpp"

#include <array>

#include "wfc/brun71/cp437.hpp"

namespace wfc::brun71 {
namespace {

// Windows virtual-key codes (winuser.h), named here so the translation
// builds and is tested on every platform.
constexpr std::uint16_t kVkTab = 0x09;
constexpr std::uint16_t kVkShift = 0x10;
constexpr std::uint16_t kVkControl = 0x11;
constexpr std::uint16_t kVkMenu = 0x12;
constexpr std::uint16_t kVkCapital = 0x14;
constexpr std::uint16_t kVkPrior = 0x21;
constexpr std::uint16_t kVkNext = 0x22;
constexpr std::uint16_t kVkEnd = 0x23;
constexpr std::uint16_t kVkHome = 0x24;
constexpr std::uint16_t kVkLeft = 0x25;
constexpr std::uint16_t kVkUp = 0x26;
constexpr std::uint16_t kVkRight = 0x27;
constexpr std::uint16_t kVkDown = 0x28;
constexpr std::uint16_t kVkInsert = 0x2D;
constexpr std::uint16_t kVkDelete = 0x2E;
constexpr std::uint16_t kVkF1 = 0x70;
constexpr std::uint16_t kVkF10 = 0x79;
constexpr std::uint16_t kVkF11 = 0x7A;
constexpr std::uint16_t kVkF12 = 0x7B;
constexpr std::uint16_t kVkNumLock = 0x90;
constexpr std::uint16_t kVkScroll = 0x91;
constexpr std::uint16_t kVkOemPlus = 0xBB;
constexpr std::uint16_t kVkOemMinus = 0xBD;

// The BIOS codes of a key: alone, with Shift, with Ctrl, with Alt.
struct Codes {
    std::uint16_t virtual_key;
    std::array<std::uint8_t, 4> code;
};

constexpr std::array<Codes, 12> kEditing = {{
    {kVkHome, {71, 71, 119, 151}},
    {kVkUp, {72, 72, 141, 152}},
    {kVkPrior, {73, 73, 132, 153}},
    {kVkLeft, {75, 75, 115, 155}},
    {kVkRight, {77, 77, 116, 157}},
    {kVkEnd, {79, 79, 117, 159}},
    {kVkDown, {80, 80, 145, 154}},
    {kVkNext, {81, 81, 118, 161}},
    {kVkInsert, {82, 82, 146, 162}},
    {kVkDelete, {83, 83, 147, 163}},
    {kVkF11, {133, 135, 137, 139}},
    {kVkF12, {134, 136, 138, 140}},
}};

// The scan codes of the letters A-Z, which Alt+letter returns.
constexpr std::array<std::uint8_t, 26> kLetterScan = {
    30, 48, 46, 32, 18, 33, 34, 35, 23, 36, 37, 38, 50,
    49, 24, 25, 16, 19, 31, 20, 22, 47, 17, 45, 21, 44,
};

constexpr Key extended(std::uint8_t code) {
    return Key{0, code};
}

// 0 alone, 1 Shift, 2 Ctrl, 3 Alt (Alt first, then Ctrl, then Shift).
int column(const KeyPress& press) {
    if (press.alt) {
        return 3;
    }
    if (press.control) {
        return 2;
    }
    return press.shift ? 1 : 0;
}

}  // namespace

std::string inkey_string(Key key) {
    if (key.extended()) {
        return std::string{'\0', static_cast<char>(key.code)};
    }
    return std::string(1U, static_cast<char>(key.character));
}

std::optional<Key> translate(const KeyPress& press) {
    const std::uint16_t vk = press.virtual_key;
    if (vk == kVkShift || vk == kVkControl || vk == kVkMenu ||
        vk == kVkCapital || vk == kVkNumLock || vk == kVkScroll) {
        return std::nullopt;
    }
    if (vk >= kVkF1 && vk <= kVkF10) {
        static constexpr std::array<std::uint8_t, 4> kBase = {59, 84, 94, 104};
        return extended(static_cast<std::uint8_t>(
            kBase[static_cast<std::size_t>(column(press))] + (vk - kVkF1)));
    }
    for (const Codes& codes : kEditing) {
        if (codes.virtual_key == vk) {
            return extended(
                codes.code[static_cast<std::size_t>(column(press))]);
        }
    }
    if (vk == kVkTab && press.shift && !press.control && !press.alt) {
        return extended(15);
    }
    // Alt with a letter or digit (AltGr, which Windows reports as Ctrl+Alt,
    // types a character instead).
    if (press.alt && !press.control) {
        if (vk >= 'A' && vk <= 'Z') {
            return extended(kLetterScan[static_cast<std::size_t>(vk - 'A')]);
        }
        if (vk >= '1' && vk <= '9') {
            return extended(static_cast<std::uint8_t>(120 + (vk - '1')));
        }
        if (vk == '0') {
            return extended(129);
        }
        if (vk == kVkOemMinus) {
            return extended(130);
        }
        if (vk == kVkOemPlus) {
            return extended(131);
        }
    }
    if (press.character != 0) {
        const auto code = unicode_to_cp437(press.character);
        if (code && *code != 0) {
            return Key{*code, 0};
        }
    }
    return std::nullopt;
}

bool KeyBuffer::push(Key key) {
    if (keys_.size() >= kCapacity) {
        return false;
    }
    keys_.push_back(key);
    return true;
}

std::optional<Key> KeyBuffer::pop() {
    if (keys_.empty()) {
        return std::nullopt;
    }
    const Key key = keys_.front();
    keys_.pop_front();
    return key;
}

std::string KeyBuffer::inkey() {
    const auto key = pop();
    return key ? inkey_string(*key) : std::string();
}

}  // namespace wfc::brun71
