// The mouse of a text user interface: what the user does with the mouse,
// at a character cell (milestone M3, WFC ADR-0012). The Windows console
// reports the state of the buttons and where the pointer is; mouse_changes()
// turns two such reports into the events a program sees.
#pragma once

#include <optional>
#include <string_view>
#include <vector>

namespace wfc::brun71 {

// The buttons, as VB numbers them.
inline constexpr int kLeftButton = 1;
inline constexpr int kRightButton = 2;
inline constexpr int kMiddleButton = 4;

struct MouseEvent {
    enum class Kind { down, up, move, double_click };
    Kind kind = Kind::move;
    // The button that went down or up; for a move, every button held.
    int buttons = 0;
    // The cell under the pointer (1-based).
    int row = 1;
    int column = 1;
    bool shift = false;
    bool control = false;
    bool alt = false;

    bool operator==(const MouseEvent&) const = default;
};

// The events between two reports of the mouse: each button that went down
// (a double click when `double_click` is set and it is the left one), each
// that went up, then a move when `moved` is set and no button changed.
// `before` and `after` are the buttons held (kLeftButton | ...).
std::vector<MouseEvent> mouse_changes(int before, int after, bool moved,
                                      bool double_click, int row, int column);

// A mouse record in a key stream (a file of keys for a test): after the two
// bytes 0, 255 come four bytes, the kind ('D' down, 'U' up, 'M' move, '2'
// double click), the buttons, the row and the column. nullopt when the four
// bytes are not one.
std::optional<MouseEvent> mouse_record(std::string_view bytes);

}  // namespace wfc::brun71
