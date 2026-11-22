// The soft keys of BASIC PDS 7.1: KEY n, text$, KEY ON, KEY OFF, KEY LIST.
//
// Each of the function keys F1 to F10, F11 and F12 has a text of up to 15
// characters (a compiled program starts with all of them empty). KEY ON shows
// them on the bottom line of the screen, the line outside the default text
// viewport: for each key its number and the first 6 characters of its text
// (control characters as blanks) in reverse video, 8 columns a key, as many
// keys as fit (10 on 80 columns, 5 on 40). KEY OFF clears the line. KEY LIST
// prints every text, one a line, as "F1 text".
#pragma once

#include <array>
#include <string>
#include <string_view>

#include "wfc/brun71/screen.hpp"

namespace wfc::brun71 {

class SoftKeys {
public:
    static constexpr int kKeys = 12;
    static constexpr std::size_t kLimit = 15;

    // KEY n, text$: n is 1 to 12; error 5 otherwise. The text is cut to 15
    // characters. Redraws the line when it is shown.
    void assign(int key, std::string_view text, TextScreen& screen);

    // KEY ON, KEY OFF.
    void show(TextScreen& screen);
    void hide(TextScreen& screen);
    bool shown() const noexcept { return shown_; }

    // Draws the line again (after WIDTH or CLS cleared it) when it is shown.
    void redraw(TextScreen& screen) const;

    // KEY LIST: the lines it prints.
    std::array<std::string, kKeys> list() const;

    const std::string& text(int key) const {
        return texts_.at(static_cast<std::size_t>(key - 1));
    }

private:
    std::array<std::string, kKeys> texts_{};
    bool shown_ = false;
};

}  // namespace wfc::brun71
