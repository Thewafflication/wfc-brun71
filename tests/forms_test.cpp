// TC-0012: drawing a text user interface (tests/tc-0012-forms.tex).
#include <string>

#include "check.hpp"
#include "wfc/brun71/forms.hpp"

using wfc::brun71::Border;
using wfc::brun71::Cell;
using wfc::brun71::Region;
using wfc::brun71::TextScreen;

namespace {

std::string row_text(const TextScreen& screen, int row, int from, int count) {
    std::string text;
    for (int column = from; column < from + count; ++column) {
        text.push_back(static_cast<char>(screen.cell(row, column).character));
    }
    return text;
}

void boxes() {
    TextScreen screen;
    const Region box{2, 3, 4, 6};
    wfc::brun71::fill(screen, box, 0x70);
    wfc::brun71::frame(screen, box, Border::double_line, 0x7F);
    CHECK(row_text(screen, 2, 3, 6) == "\xC9\xCD\xCD\xCD\xCD\xBB");
    CHECK(row_text(screen, 3, 3, 6) == "\xBA    \xBA");
    CHECK(row_text(screen, 5, 3, 6) == "\xC8\xCD\xCD\xCD\xCD\xBC");
    CHECK(screen.cell(2, 3).attribute == 0x7F);
    CHECK(screen.cell(3, 4).attribute == 0x70);
    wfc::brun71::frame(screen, box, Border::single, 0x70);
    CHECK(row_text(screen, 2, 3, 6) == "\xDA\xC4\xC4\xC4\xC4\xBF");
    CHECK(row_text(screen, 5, 3, 1) == "\xC0");
    // A region off the screen draws only its cells on the screen.
    wfc::brun71::frame(screen, Region{24, 78, 4, 5}, Border::single, 0x70);
    CHECK(row_text(screen, 24, 78, 3) == "\xDA\xC4\xC4");
    wfc::brun71::fill(screen, Region{-3, -3, 2, 2}, 0x70, 'x');
    CHECK(screen.cell(1, 1).character == ' ');
}

void titles_and_shadows() {
    TextScreen screen;
    const Region box{1, 1, 3, 12};
    wfc::brun71::frame(screen, box, Border::single, 0x70);
    wfc::brun71::title(screen, box, "Hello", 0x70);
    CHECK(row_text(screen, 1, 1, 12) == "\xDA\xC4 Hello \xC4\xC4\xBF");
    wfc::brun71::title(screen, Region{5, 1, 3, 6}, "Long title", 0x70);
    CHECK(row_text(screen, 5, 2, 4) == " Lon");
    screen.put(2, 13, Cell{'a', 7});
    wfc::brun71::shadow(screen, box);
    CHECK(screen.cell(1, 13).attribute == 7);  // the first row has none
    CHECK(screen.cell(2, 13).attribute == wfc::brun71::kShadowAttribute);
    CHECK(screen.cell(2, 13).character == 'a');
    CHECK(screen.cell(4, 14).attribute == wfc::brun71::kShadowAttribute);
    CHECK(screen.cell(4, 2).attribute == 7);
    CHECK(screen.cell(4, 3).attribute == wfc::brun71::kShadowAttribute);
}

void texts_and_captions() {
    TextScreen screen;
    CHECK(wfc::brun71::text(screen, 3, 78, "abcdef", 0x70, 4) == 4);
    CHECK(row_text(screen, 3, 78, 3) == "abc");
    CHECK(wfc::brun71::text(screen, 4, 1, "ab", 0x70, 0) == 0);
    const auto open = wfc::brun71::caption("&Open");
    CHECK(open.text == "Open" && open.access == 0);
    const auto save = wfc::brun71::caption("Save &As");
    CHECK(save.text == "Save As" && save.access == 5);
    const auto both = wfc::brun71::caption("A && B");
    CHECK(both.text == "A & B" && both.access == -1);
    CHECK(wfc::brun71::caption("End&").text == "End&");
    CHECK(wfc::brun71::button_face("OK", 10) == "  < OK >");
    CHECK(wfc::brun71::button_face("Cancel", 6) == "< Canc");
    CHECK(wfc::brun71::button_face("OK", 0).empty());
}

}  // namespace

int main() {
    boxes();
    titles_and_shadows();
    texts_and_captions();
    return wfc::brun71::test::finish("forms");
}
