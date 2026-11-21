// The text screen of BASIC PDS 7.1 (SCREEN 0): a grid of character cells,
// each a code page 437 character and a CGA attribute, with the cursor, the
// colours and the text viewport that the console statements work on.
//
// Rows and columns are 1-based, as in BASIC. The model draws nothing; a host
// (console.hpp on Windows, or a test) shows the cells. Every statement checks
// its arguments as BRUN71 does and raises error 5 (BasicError) otherwise; an
// argument that BASIC lets the program leave out is a std::optional.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string_view>
#include <vector>

namespace wfc::brun71 {

// One character cell: the code page 437 character and the attribute
// (bits 0-3 foreground, 4-6 background, 7 blink), as in CGA text memory.
struct Cell {
    std::uint8_t character = 32;
    std::uint8_t attribute = 7;

    bool operator==(const Cell&) const = default;
};

// The attribute of COLOR foreground, background: foreground 0-31 (16-31
// blink), background 0-7 (the high bit is the blink bit, not a colour).
std::uint8_t make_attribute(int foreground, int background);

class TextScreen {
public:
    // WIDTH 80 on a 25-line screen, COLOR 7, 0, the default viewport.
    TextScreen();

    int columns() const noexcept { return columns_; }
    int rows() const noexcept { return rows_; }

    // --- Statements ---------------------------------------------------------

    // WIDTH columns, rows: 40 or 80 columns, 25, 43 or 50 rows. Clears the
    // screen and resets the viewport; the cursor goes home.
    void width(std::optional<int> columns, std::optional<int> rows);

    // COLOR foreground, background, border (SCREEN 0): foreground 0-31,
    // background 0-7, border 0-15; the border is kept but a console has none
    // to show.
    void color(std::optional<int> foreground, std::optional<int> background,
               std::optional<int> border = std::nullopt);

    // LOCATE row, column, cursor, start, stop. The row is a line of the text
    // viewport or the bottom line of the screen; cursor is 0 or 1; start and
    // stop are the scan lines 0-31 of its shape (stop defaults to start).
    void locate(std::optional<int> row, std::optional<int> column,
                std::optional<int> cursor = std::nullopt,
                std::optional<int> start = std::nullopt,
                std::optional<int> stop = std::nullopt);

    // VIEW PRINT top TO bottom; without the two lines, the whole screen. (A
    // program starts, and WIDTH leaves it, with every line but the bottom one,
    // which BASIC keeps for the soft keys.) The cursor goes to the top line of
    // the viewport.
    void view_print(std::optional<int> top = std::nullopt,
                    std::optional<int> bottom = std::nullopt);

    // CLS [0 | 1 | 2]. 0 clears the whole screen; with no graphics viewport
    // in text mode, 1 does too; 2 and no argument clear the text viewport.
    // The cleared cells take the current attribute; the cursor goes to the
    // top line of the viewport, column 1.
    void cls(std::optional<int> mode = std::nullopt);

    // --- PRINT --------------------------------------------------------------

    // Writes text at the cursor as PRINT does: a line that is full wraps, the
    // viewport scrolls up at its bottom line, and the control characters
    // BASIC acts on move the cursor instead of printing (7 bell, 9 tab to the
    // next multiple of 8, 10 and 13 new line, 11 home, 12 clear the viewport,
    // 28 right, 29 left, 30 up, 31 down). Other codes below 32 print their
    // glyphs.
    void print_text(std::string_view text);

    // One PRINT item: text that would not fit on the rest of the line, but
    // fits on a line of its own, starts on the next line.
    void print_item(std::string_view text);

    // The end of a PRINT without a trailing separator: a new line.
    void new_line();

    // A comma between PRINT items: the next 14-column print zone, or the next
    // line when no zone is left on this one.
    void comma();

    // TAB(column): moves to the column, on the next line when the cursor is
    // already past it. Columns past the width wrap (modulo the width); a
    // column below 1 is 1.
    void tab(int column);

    // SPC(count): prints count spaces (modulo the width); below 0 prints none.
    void spc(int count);

    // --- Functions ----------------------------------------------------------

    int csrlin() const noexcept { return row_; }
    int pos() const noexcept { return column_; }

    // SCREEN(row, column [, colorflag]): the character code of the cell, or
    // its attribute when colorflag is not 0.
    int screen(int row, int column, int colorflag = 0) const;

    // --- Cells and state, for the host and the UI toolbox -------------------

    const Cell& cell(int row, int column) const;
    // Writes one cell without moving the cursor (the toolbox draws this way).
    void put(int row, int column, Cell cell);
    // Every cell, row by row (row 1 first).
    const std::vector<Cell>& cells() const noexcept { return cells_; }

    std::uint8_t attribute() const noexcept { return attribute_; }
    int foreground() const noexcept { return foreground_; }
    int background() const noexcept { return background_; }
    int border() const noexcept { return border_; }
    int view_top() const noexcept { return view_top_; }
    int view_bottom() const noexcept { return view_bottom_; }
    bool cursor_visible() const noexcept { return cursor_visible_; }
    int cursor_start() const noexcept { return cursor_start_; }
    int cursor_stop() const noexcept { return cursor_stop_; }

    // Called for CHR$(7). The default does nothing; a host beeps.
    std::function<void()> on_bell;

private:
    void reset(int columns, int rows);
    void clear_rows(int top, int bottom);
    void scroll_view();
    void advance_line();
    void write_glyph(std::uint8_t character);
    void control(std::uint8_t character);
    Cell& at(int row, int column);

    int columns_ = 80;
    int rows_ = 25;
    std::vector<Cell> cells_;
    int row_ = 1;
    int column_ = 1;
    int view_top_ = 1;
    int view_bottom_ = 24;
    int foreground_ = 7;
    int background_ = 0;
    int border_ = 0;
    std::uint8_t attribute_ = 7;
    bool cursor_visible_ = false;
    int cursor_start_ = 6;
    int cursor_stop_ = 7;
};

}  // namespace wfc::brun71
