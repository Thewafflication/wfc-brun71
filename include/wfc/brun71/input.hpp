// The keyboard input statements of BASIC PDS 7.1: the line editor of INPUT
// and LINE INPUT, and how INPUT splits the line it read into fields.
//
// INPUT [;] ["prompt" {; | ,}] variable [, variable ...]
// LINE INPUT [;] ["prompt";] variable$
//
// The statement prints its prompt (INPUT adds "? " after a prompt followed by
// a semicolon, or alone), then the user types a line: the characters show at
// the cursor, Backspace takes the last one back, Esc the whole line, and
// Enter ends it. INPUT then splits the line at its commas; when the number of
// fields or a number is wrong it prints "Redo from start" and asks again.
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "wfc/brun71/keyboard.hpp"
#include "wfc/brun71/screen.hpp"

namespace wfc::brun71 {

// What INPUT prints before it asks again.
inline constexpr std::string_view kRedoFromStart = "Redo from start";

// The line the user types, shown on the screen at the cursor.
class LineEditor {
public:
    // The longest line a user can type; further characters are refused.
    static constexpr std::size_t kLimit = 255;

    explicit LineEditor(TextScreen& screen) : screen_(screen) {}

    // Takes one key. A character from 32 up is typed; Backspace (8) erases
    // the last one, Esc (27) the whole line; other keys are ignored. Returns
    // true at Enter (13), which ends the line and moves nothing.
    bool key(Key key);

    // The bytes typed (code page 437).
    const std::string& text() const noexcept { return text_; }

private:
    void erase_last();

    TextScreen& screen_;
    std::string text_;
};

// One field of an INPUT line.
struct InputField {
    std::string text;
    // In quotes: kept as typed, commas and blanks included.
    bool quoted = false;
};

// Splits an INPUT line at its commas. A field is either quoted ("...",
// followed only by blanks before the next comma) or the text up to the next
// comma with the blanks around it removed. A line with text after a closing
// quote gives nullopt.
std::optional<std::vector<InputField>> split_input(std::string_view line);

// The number an INPUT field holds: blanks around it, a sign, decimal digits
// with a point and an exponent (E or D), or &H hexadecimal and &O (or &)
// octal digits, then at most one type suffix (! # % &). An empty field is 0.
// nullopt when the field is not a number.
std::optional<double> parse_input_number(std::string_view field);

}  // namespace wfc::brun71
