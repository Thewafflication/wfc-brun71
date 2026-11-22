// Drawing the windows and controls of a text user interface on a TextScreen
// (milestone M3, WFC ADR-0012): boxes, titles, shadows, text clipped to a
// width, and the captions of controls with their access keys. WFC's text
// host draws VB6 forms with these; nothing here knows about VB6.
//
// Rows and columns are 1-based, as on the screen. A region may lie partly
// or wholly off the screen: only its cells on the screen are drawn. Text is
// code page 437 bytes.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "wfc/brun71/screen.hpp"

namespace wfc::brun71 {

// A rectangle of cells: its top row, left column, height and width.
struct Region {
    int top = 1;
    int left = 1;
    int height = 0;
    int width = 0;

    int bottom() const noexcept { return top + height - 1; }
    int right() const noexcept { return left + width - 1; }
};

enum class Border { single, double_line };

// Attributes of the text user interface (CGA: background * 16 + foreground).
inline constexpr std::uint8_t kWindowAttribute = 0x70;  // black on grey
inline constexpr std::uint8_t kActiveFrame = 0x7F;      // white on grey
inline constexpr std::uint8_t kFocusAttribute = 0x1F;   // white on blue
inline constexpr std::uint8_t kFieldAttribute = 0x30;   // black on cyan
inline constexpr std::uint8_t kShadowAttribute = 0x08;  // dark grey on black

// Writes one cell when it is on the screen.
void put_clipped(TextScreen& screen, int row, int column, Cell cell);

// Fills the region with a character in an attribute.
void fill(TextScreen& screen, const Region& region, std::uint8_t attribute,
          std::uint8_t character = ' ');

// Draws the border of the region (its outer cells) with single or double
// lines; the inside is left as it is.
void frame(TextScreen& screen, const Region& region, Border border,
           std::uint8_t attribute);

// Writes " title " centred on the top row of the region, between its
// corners; a title too long for the row is cut.
void title(TextScreen& screen, const Region& region, std::string_view text,
           std::uint8_t attribute);

// The shadow of a window: the column pair to the right of the region (from
// its second row) and the row below it (from its third column) keep their
// characters in kShadowAttribute.
void shadow(TextScreen& screen, const Region& region);

// Writes text from (row, column), at most `width` cells; returns how many
// it wrote.
int text(TextScreen& screen, int row, int column, std::string_view bytes,
         std::uint8_t attribute, int width);

// A control's caption without its access-key marks: "&Open" is "Open" with
// the access key at index 0; "&&" is a single "&". `access` is -1 when the
// caption has no access key.
struct Caption {
    std::string text;
    int access = -1;
};
Caption caption(std::string_view marked);

// The face of a command button: "< text >" centred in `width` cells (cut
// to the width when it does not fit).
std::string button_face(std::string_view text, int width);

}  // namespace wfc::brun71
