// TC-0002: the keys INKEY$ returns and the type-ahead buffer
// (tests/tc-0002-keyboard.tex).
#include <string>

#include "check.hpp"
#include "wfc/brun71/keyboard.hpp"

using wfc::brun71::inkey_string;
using wfc::brun71::Key;
using wfc::brun71::KeyBuffer;
using wfc::brun71::KeyPress;
using wfc::brun71::translate;

namespace {

KeyPress press(std::uint16_t vk, char32_t character = 0, bool shift = false,
               bool control = false, bool alt = false) {
    KeyPress result;
    result.virtual_key = vk;
    result.character = character;
    result.shift = shift;
    result.control = control;
    result.alt = alt;
    return result;
}

// The BIOS code of an extended key press, or -1 when it is not extended.
int code_of(const KeyPress& value) {
    const auto key = translate(value);
    return key && key->extended() ? key->code : -1;
}

void strings() {
    CHECK(inkey_string(Key{'A', 0}) == "A");
    CHECK(inkey_string(Key{0, 72}) == std::string("\0H", 2));
    CHECK(inkey_string(Key{13, 0}) == "\r");
}

void characters() {
    CHECK(translate(press('A', U'a')) == (Key{'a', 0}));
    CHECK(translate(press('A', U'A', true)) == (Key{'A', 0}));
    CHECK(translate(press(0x0D, U'\r')) == (Key{13, 0}));
    CHECK(translate(press(0x1B, 27)) == (Key{27, 0}));
    CHECK(translate(press(0x08, 8)) == (Key{8, 0}));
    CHECK(translate(press('C', 3, false, true)) == (Key{3, 0}));
    CHECK(translate(press(0xDE, char32_t{0x00E9})) == (Key{130, 0}));
    // AltGr (Ctrl+Alt) types its character.
    CHECK(translate(press('Q', U'@', false, true, true)) == (Key{'@', 0}));
    CHECK(!translate(press('E', char32_t{0x20AC}, false, true, true)));
    CHECK(!translate(press(0x10, 0, true)));
    CHECK(!translate(press(0x11, 0, false, true)));
    CHECK(!translate(press(0x12, 0, false, false, true)));
    CHECK(!translate(press(0x14)));
}

void extended_keys() {
    CHECK(code_of(press(0x26)) == 72);
    CHECK(code_of(press(0x28)) == 80);
    CHECK(code_of(press(0x25)) == 75);
    CHECK(code_of(press(0x27)) == 77);
    CHECK(code_of(press(0x24)) == 71);
    CHECK(code_of(press(0x23)) == 79);
    CHECK(code_of(press(0x21)) == 73);
    CHECK(code_of(press(0x22)) == 81);
    CHECK(code_of(press(0x2D)) == 82);
    CHECK(code_of(press(0x2E)) == 83);
    CHECK(code_of(press(0x25, 0, false, true)) == 115);
    CHECK(code_of(press(0x27, 0, false, true)) == 116);
    CHECK(code_of(press(0x24, 0, false, true)) == 119);
    CHECK(code_of(press(0x26, 0, false, false, true)) == 152);

    CHECK(code_of(press(0x70)) == 59);
    CHECK(code_of(press(0x79)) == 68);
    CHECK(code_of(press(0x70, 0, true)) == 84);
    CHECK(code_of(press(0x79, 0, false, true)) == 103);
    CHECK(code_of(press(0x70, 0, false, false, true)) == 104);
    CHECK(code_of(press(0x7A)) == 133);
    CHECK(code_of(press(0x7B)) == 134);
    CHECK(code_of(press(0x7B, 0, false, false, true)) == 140);

    CHECK(code_of(press(0x09, 9, true)) == 15);
    CHECK(translate(press(0x09, 9)) == (Key{9, 0}));

    CHECK(code_of(press('X', U'x', false, false, true)) == 45);
    CHECK(code_of(press('A', U'a', false, false, true)) == 30);
    CHECK(code_of(press('Q', U'q', false, false, true)) == 16);
    CHECK(code_of(press('M', U'm', false, false, true)) == 50);
    CHECK(code_of(press('1', U'1', false, false, true)) == 120);
    CHECK(code_of(press('0', U'0', false, false, true)) == 129);
    CHECK(code_of(press(0xBD, U'-', false, false, true)) == 130);
    CHECK(code_of(press(0xBB, U'=', false, false, true)) == 131);
}

void buffer() {
    KeyBuffer keys;
    CHECK(keys.inkey().empty());
    for (int i = 0; i < 15; ++i) {
        CHECK(keys.push(Key{static_cast<std::uint8_t>('a' + i), 0}));
    }
    CHECK(!keys.push(Key{'z', 0}));
    CHECK(keys.size() == 15U);
    CHECK(keys.inkey() == "a");
    CHECK(keys.push(Key{0, 59}));
    for (int i = 1; i < 15; ++i) {
        CHECK(keys.inkey() == std::string(1, static_cast<char>('a' + i)));
    }
    CHECK(keys.inkey() == std::string("\0;", 2));
    CHECK(keys.empty());
    CHECK(!keys.pop());
    keys.push(Key{'x', 0});
    keys.clear();
    CHECK(keys.inkey().empty());
}

}  // namespace

int main() {
    strings();
    characters();
    extended_keys();
    buffer();
    return wfc::brun71::test::finish("keyboard");
}
