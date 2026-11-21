// TC-0004: the Windows console host (tests/tc-0004-console-host.tex).
// The screen buffer is never made active, so the console the test runs in
// is not disturbed. Exits 77 (skipped) when the process has no console.
#include <cstdio>

#include "check.hpp"
#include "wfc/brun71/console.hpp"

using wfc::brun71::Console;
using wfc::brun71::TextScreen;

int main() {
    const auto console = Console::open(false);
    if (!console) {
        std::printf("console: skipped (no console)\n");
        return 77;
    }

    TextScreen screen;
    screen.color(14, 1);
    screen.locate(3, 5);
    screen.print_text(
        "\xC9"
        "A\x01");
    console->present(screen);
    auto shown = console->shown(3, 5);
    CHECK(shown.character == char32_t{0x2554});
    CHECK(shown.attribute == 0x1E);
    CHECK(console->shown(3, 6).character == U'A');
    CHECK(console->shown(3, 7).character == char32_t{0x263A});
    CHECK(console->shown(1, 1).character == U' ');
    CHECK(console->shown(1, 1).attribute == 0x07);
    auto cursor = console->cursor();
    CHECK(cursor.row == 3 && cursor.column == 8);
    CHECK(!cursor.visible);

    // Blink has no console equivalent: the attribute keeps the colours.
    screen.color(30, 1);
    screen.locate(25, 80);
    screen.print_text("B");
    screen.locate(10, 1, 1, 0, 7);
    console->present(screen);
    shown = console->shown(25, 80);
    CHECK(shown.character == U'B');
    CHECK(shown.attribute == 0x1E);
    cursor = console->cursor();
    CHECK(cursor.row == 10 && cursor.column == 1);
    CHECK(cursor.visible && cursor.size == 100);

    // A 40-column screen.
    screen.width(40, std::nullopt);
    screen.locate(1, 40);
    screen.color(2, 0);
    screen.print_text("\xDB");
    console->present(screen);
    shown = console->shown(1, 40);
    CHECK(shown.character == char32_t{0x2588});
    CHECK(shown.attribute == 0x02);
    return wfc::brun71::test::finish("console");
}
