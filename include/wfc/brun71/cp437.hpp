// Code page 437, the character set of the PC text screen: the BASIC strings
// a PDS program prints are bytes of this code page, which a host shows as
// Unicode. Codes 1-31 and 127 are the screen glyphs (smiley, arrows, house),
// since that is what the text screen shows for them; code 0 shows as a space.
#pragma once

#include <cstdint>
#include <optional>

namespace wfc::brun71 {

// The Unicode character the screen shows for a code page 437 byte.
char32_t cp437_to_unicode(std::uint8_t code);

// The code page 437 byte of a Unicode character typed or pasted, if it has
// one. Control characters 0-31 map to themselves (keys such as Enter, Esc
// and Ctrl+letter are typed as those codes).
std::optional<std::uint8_t> unicode_to_cp437(char32_t character);

}  // namespace wfc::brun71
