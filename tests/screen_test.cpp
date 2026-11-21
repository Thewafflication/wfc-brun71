// TC-0001: the text screen rules (tests/tc-0001-text-screen.tex).
#include <string>

#include "check.hpp"
#include "wfc/brun71/screen.hpp"

using wfc::brun71::Cell;
using wfc::brun71::kIllegalFunctionCall;
using wfc::brun71::TextScreen;

namespace {

// The characters of a row from a column on, as text.
std::string text_at(const TextScreen& screen, int row, int column, int count) {
    std::string text;
    for (int i = 0; i < count; ++i) {
        text += static_cast<char>(screen.cell(row, column + i).character);
    }
    return text;
}

void defaults() {
    TextScreen screen;
    CHECK(screen.columns() == 80 && screen.rows() == 25);
    CHECK(screen.csrlin() == 1 && screen.pos() == 1);
    CHECK(screen.view_top() == 1 && screen.view_bottom() == 24);
    CHECK(screen.attribute() == 7);
    CHECK(!screen.cursor_visible());
    CHECK(screen.cell(25, 80) == (Cell{32, 7}));
}

void printing() {
    TextScreen screen;
    screen.print_item("HELLO");
    CHECK(text_at(screen, 1, 1, 5) == "HELLO");
    CHECK(screen.pos() == 6);
    screen.new_line();
    CHECK(screen.csrlin() == 2 && screen.pos() == 1);

    // A full line wraps at once.
    screen.locate(1, 78);
    screen.print_text("ABCDE");
    CHECK(text_at(screen, 1, 78, 3) == "ABC");
    CHECK(text_at(screen, 2, 1, 2) == "DE");
    CHECK(screen.csrlin() == 2 && screen.pos() == 3);

    // An item that does not fit starts on the next line.
    screen.locate(5, 75);
    screen.print_item("ABCDEFGHIJ");
    CHECK(text_at(screen, 6, 1, 10) == "ABCDEFGHIJ");
    CHECK(screen.cell(5, 75).character == 32);

    // ... unless it is longer than a line, or the cursor is in column 1.
    screen.locate(8, 1);
    screen.print_item(std::string(85, 'x'));
    CHECK(screen.csrlin() == 9 && screen.pos() == 6);
}

void scrolling() {
    TextScreen screen;
    screen.locate(25, 1);
    screen.print_text("Z");
    screen.locate(24, 1);
    screen.print_text("X");
    screen.new_line();
    CHECK(screen.csrlin() == 24 && screen.pos() == 1);
    CHECK(screen.cell(23, 1).character == 'X');
    CHECK(screen.cell(25, 1).character == 'Z');

    // From the bottom line, a new line scrolls the viewport.
    screen.locate(25, 2);
    screen.new_line();
    CHECK(screen.csrlin() == 24);
    CHECK(screen.cell(22, 1).character == 'X');
    CHECK(screen.cell(25, 1).character == 'Z');

    // The last cell of the viewport scrolls it.
    TextScreen corner;
    corner.locate(24, 80);
    corner.print_text("Q");
    CHECK(corner.cell(23, 80).character == 'Q');
    CHECK(corner.csrlin() == 24 && corner.pos() == 1);
}

void separators() {
    TextScreen screen;
    screen.comma();
    CHECK(screen.pos() == 15);
    screen.print_text("A");
    screen.comma();
    CHECK(screen.pos() == 29);
    screen.locate(3, 71);
    screen.comma();
    CHECK(screen.csrlin() == 4 && screen.pos() == 1);

    screen.tab(10);
    CHECK(screen.pos() == 10);
    screen.locate(6, 20);
    screen.tab(10);
    CHECK(screen.csrlin() == 7 && screen.pos() == 10);
    screen.locate(8, 1);
    screen.tab(85);
    CHECK(screen.pos() == 5);
    screen.tab(0);
    CHECK(screen.csrlin() == 9 && screen.pos() == 1);

    screen.spc(3);
    CHECK(screen.pos() == 4);
    screen.spc(-1);
    CHECK(screen.pos() == 4);
    screen.spc(83);
    CHECK(screen.pos() == 7);
}

void controls() {
    TextScreen screen;
    int bells = 0;
    screen.on_bell = [&bells] {
        ++bells;
    };
    screen.print_text("\a");
    CHECK(bells == 1 && screen.pos() == 1);
    screen.print_text("A\t");
    CHECK(screen.pos() == 9);
    screen.print_text("B\rC");
    CHECK(screen.csrlin() == 2 && screen.pos() == 2);
    screen.print_text("\x1c\x1c\x1d");
    CHECK(screen.pos() == 3);
    screen.print_text("\x1f\x1f\x1e");
    CHECK(screen.csrlin() == 3);
    screen.print_text("\v");
    CHECK(screen.csrlin() == 1 && screen.pos() == 1);
    screen.print_text("\x01\x7f");
    CHECK(screen.cell(1, 1).character == 1);
    CHECK(screen.cell(1, 2).character == 127);
    screen.print_text("\f");
    CHECK(screen.cell(1, 1).character == 32 && screen.csrlin() == 1);
}

void colours() {
    TextScreen screen;
    screen.color(14, 1);
    CHECK(screen.attribute() == 0x1E);
    screen.print_text("A");
    CHECK(screen.cell(1, 1) == (Cell{'A', 0x1E}));
    screen.color(31, std::nullopt);
    CHECK(screen.attribute() == 0x9F);
    screen.color(std::nullopt, 4, 3);
    CHECK(screen.attribute() == 0xCF && screen.border() == 3);
    CHECK_ERROR(kIllegalFunctionCall, screen.color(32, std::nullopt));
    CHECK_ERROR(kIllegalFunctionCall, screen.color(std::nullopt, 8));
    CHECK_ERROR(kIllegalFunctionCall, screen.color(-1, std::nullopt));
    CHECK_ERROR(kIllegalFunctionCall, screen.color(1, 1, 16));
    CHECK(screen.attribute() == 0xCF);
}

void locating() {
    TextScreen screen;
    screen.locate(10, 20);
    CHECK(screen.csrlin() == 10 && screen.pos() == 20);
    screen.locate(std::nullopt, 5);
    CHECK(screen.csrlin() == 10 && screen.pos() == 5);
    screen.locate(25, std::nullopt);
    CHECK(screen.csrlin() == 25);
    CHECK_ERROR(kIllegalFunctionCall, screen.locate(0, 1));
    CHECK_ERROR(kIllegalFunctionCall, screen.locate(26, 1));
    CHECK_ERROR(kIllegalFunctionCall, screen.locate(1, 81));
    CHECK_ERROR(kIllegalFunctionCall, screen.locate(1, 0));
    CHECK_ERROR(kIllegalFunctionCall,
                screen.locate(std::nullopt, std::nullopt, 2));
    CHECK_ERROR(kIllegalFunctionCall,
                screen.locate(std::nullopt, std::nullopt, 1, 32));

    screen.locate(std::nullopt, std::nullopt, 1, 0, 7);
    CHECK(screen.cursor_visible());
    CHECK(screen.cursor_start() == 0 && screen.cursor_stop() == 7);
    screen.locate(std::nullopt, std::nullopt, std::nullopt, 4);
    CHECK(screen.cursor_start() == 4 && screen.cursor_stop() == 4);
    screen.locate(std::nullopt, std::nullopt, 0);
    CHECK(!screen.cursor_visible());
}

void viewports() {
    TextScreen screen;
    screen.locate(1, 1);
    screen.print_text("TOP");
    screen.view_print(5, 10);
    CHECK(screen.view_top() == 5 && screen.view_bottom() == 10);
    CHECK(screen.csrlin() == 5 && screen.pos() == 1);
    CHECK_ERROR(kIllegalFunctionCall, screen.locate(3, 1));
    CHECK_ERROR(kIllegalFunctionCall, screen.locate(11, 1));
    screen.locate(25, 1);
    screen.locate(5, 1);
    for (int i = 0; i < 7; ++i) {
        screen.print_item(std::string(1, static_cast<char>('a' + i)));
        screen.new_line();
    }
    // a-g, each followed by a new line, in the six lines 5-10: the last two
    // new lines scroll, so c-g are left on lines 5-9.
    CHECK(screen.cell(5, 1).character == 'c');
    CHECK(screen.cell(10, 1).character == 32);
    CHECK(screen.cell(9, 1).character == 'g');
    CHECK(text_at(screen, 1, 1, 3) == "TOP");

    screen.cls();
    CHECK(screen.cell(5, 1).character == 32);
    CHECK(text_at(screen, 1, 1, 3) == "TOP");
    CHECK(screen.csrlin() == 5);

    CHECK_ERROR(kIllegalFunctionCall, screen.view_print(5, std::nullopt));
    CHECK_ERROR(kIllegalFunctionCall, screen.view_print(0, 3));
    CHECK_ERROR(kIllegalFunctionCall, screen.view_print(10, 5));
    CHECK_ERROR(kIllegalFunctionCall, screen.view_print(1, 26));

    screen.view_print();
    CHECK(screen.view_top() == 1 && screen.view_bottom() == 25);
    CHECK(screen.csrlin() == 1);
}

void clearing() {
    TextScreen screen;
    screen.locate(25, 1);
    screen.print_text("K");
    screen.color(7, 1);
    screen.cls();
    CHECK(screen.cell(1, 1) == (Cell{32, 0x17}));
    CHECK(screen.cell(25, 1).character == 'K');
    screen.cls(2);
    CHECK(screen.cell(25, 1).character == 'K');
    screen.cls(0);
    CHECK(screen.cell(25, 1) == (Cell{32, 0x17}));
    screen.locate(25, 1);
    screen.print_text("K");
    screen.cls(1);
    CHECK(screen.cell(25, 1).character == 32);
    CHECK(screen.csrlin() == 1 && screen.pos() == 1);
    CHECK_ERROR(kIllegalFunctionCall, screen.cls(3));
    CHECK_ERROR(kIllegalFunctionCall, screen.cls(-1));
}

void widths() {
    TextScreen screen;
    screen.locate(10, 10);
    screen.width(40, std::nullopt);
    CHECK(screen.columns() == 40 && screen.rows() == 25);
    CHECK(screen.csrlin() == 1 && screen.pos() == 1);
    CHECK(screen.view_bottom() == 24);
    CHECK_ERROR(kIllegalFunctionCall, screen.locate(1, 41));
    screen.width(std::nullopt, 43);
    CHECK(screen.columns() == 40 && screen.rows() == 43);
    CHECK(screen.view_bottom() == 42);
    screen.width(80, 50);
    CHECK(screen.cells().size() == 4000U);
    CHECK_ERROR(kIllegalFunctionCall, screen.width(81, std::nullopt));
    CHECK_ERROR(kIllegalFunctionCall, screen.width(std::nullopt, 26));
    CHECK(screen.columns() == 80 && screen.rows() == 50);
}

void screen_function() {
    TextScreen screen;
    screen.color(12, 2);
    screen.locate(3, 4);
    screen.print_text("\xC9");
    CHECK(screen.screen(3, 4) == 0xC9);
    CHECK(screen.screen(3, 4, 1) == 0x2C);
    CHECK(screen.screen(3, 5) == 32);
    CHECK_ERROR(kIllegalFunctionCall, (void)screen.screen(0, 1));
    CHECK_ERROR(kIllegalFunctionCall, (void)screen.screen(1, 81));
    screen.put(25, 80, Cell{'#', 0x70});
    CHECK(screen.screen(25, 80) == '#' && screen.csrlin() == 3);
    CHECK_ERROR(kIllegalFunctionCall, screen.put(26, 1, Cell{}));
}

}  // namespace

int main() {
    defaults();
    printing();
    scrolling();
    separators();
    controls();
    colours();
    locating();
    viewports();
    clearing();
    widths();
    screen_function();
    return wfc::brun71::test::finish("screen");
}
