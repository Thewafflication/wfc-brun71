// PRINT USING: values formatted by the fields of a format string.
//
// String fields: ! (the first character), \  \ (two characters plus one per
// blank between the backslashes, padded with blanks), & (the whole string).
//
// Numeric fields: # a digit, . the decimal point, , before the point a digit
// that also groups the whole part by thousands; a leading + prints the sign,
// a trailing + or - prints it after the number (- prints a blank for a
// positive one); ** fills the blanks on the left with asterisks, $$ puts a
// dollar sign before the number, **$ does both (each adds two positions, one
// of them for the $ of $$ and **$); ^^^^ (^^^^^) prints an exponent E+nn
// (E+nnn). Without a sign field a negative number takes one of the positions
// for its minus. A number too wide for its field is printed whole, after a %.
//
// _ prints the next character of the format as it is; any other character is
// printed as it is. The format is used again from its start while values are
// left; after the last value, the text up to the next field is printed.
#pragma once

#include <span>
#include <string>
#include <string_view>
#include <variant>

namespace wfc::brun71 {

// A value of the list: a number, or the bytes of a string.
using UsingValue = std::variant<double, std::string>;

// The text PRINT USING prints for the values (without the line end).
// Throws BasicError 5 when the format has no field and there is a value, 13
// when a string meets a numeric field or a number a string field.
std::string format_using(std::string_view format,
                         std::span<const UsingValue> values);

}  // namespace wfc::brun71
