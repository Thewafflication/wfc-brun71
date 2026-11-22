// TC-0009: the soft keys and their line (tests/tc-0009-soft-keys.tex).
#include <string>

#include "check.hpp"
#include "wfc/brun71/keyline.hpp"

using wfc::brun71::SoftKeys;
using wfc::brun71::TextScreen;

namespace {

std::string line(const TextScreen& screen, int from, int count) {
    std::string text;
    for (int column = from; column < from + count; ++column) {
        text.push_back(
            static_cast<char>(screen.cell(screen.rows(), column).character));
    }
    return text;
}

void assigning() {
    TextScreen screen;
    SoftKeys keys;
    keys.assign(1, "LIST", screen);
    CHECK(keys.text(1) == "LIST");
    CHECK(line(screen, 1, 8) == "        ");  // not shown yet
    keys.assign(2, "0123456789ABCDEFGH", screen);
    CHECK(keys.text(2) == "0123456789ABCDE");
    CHECK_ERROR(5, keys.assign(0, "x", screen));
    CHECK_ERROR(5, keys.assign(13, "x", screen));
    keys.assign(12, "last", screen);
    CHECK(keys.list()[0] == "F1 LIST");
    CHECK(keys.list()[2] == "F3 ");
    CHECK(keys.list()[11] == "F12 last");
}

void showing() {
    TextScreen screen;
    SoftKeys keys;
    keys.assign(1, "LIST", screen);
    keys.assign(2, std::string("RUN") + '\r', screen);
    keys.assign(10, "SCREEN", screen);
    keys.show(screen);
    CHECK(keys.shown());
    CHECK(line(screen, 1, 16) == "1LIST   2RUN    ");
    CHECK(screen.cell(25, 1).attribute == 7);
    CHECK(screen.cell(25, 2).attribute == 0x70);
    CHECK(screen.cell(25, 7).attribute == 0x70);
    CHECK(screen.cell(25, 8).attribute == 7);
    CHECK(line(screen, 73, 8) == "10SCREE ");
    CHECK(screen.cell(25, 77).attribute == 0x70);
    // Assigning redraws the shown line.
    keys.assign(1, "EDIT", screen);
    CHECK(line(screen, 1, 6) == "1EDIT ");
    // The text viewport and the cursor are not touched.
    CHECK(screen.csrlin() == 1 && screen.view_bottom() == 24);
    keys.hide(screen);
    CHECK(!keys.shown());
    CHECK(line(screen, 1, 80) == std::string(80, ' '));
    CHECK(screen.cell(25, 2).attribute == 7);
}

void narrow() {
    TextScreen screen;
    SoftKeys keys;
    keys.assign(5, "five", screen);
    keys.assign(6, "six", screen);
    screen.width(40, 25);
    keys.show(screen);
    CHECK(line(screen, 33, 8) == "5five   ");
}

}  // namespace

int main() {
    assigning();
    showing();
    narrow();
    return wfc::brun71::test::finish("soft keys");
}
