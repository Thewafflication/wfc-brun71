// TC-0006: the line editor of INPUT and LINE INPUT, and how INPUT splits its
// line (tests/tc-0006-input.tex).
#include <string>

#include "check.hpp"
#include "wfc/brun71/input.hpp"

using wfc::brun71::InputField;
using wfc::brun71::Key;
using wfc::brun71::LineEditor;
using wfc::brun71::parse_input_number;
using wfc::brun71::split_input;
using wfc::brun71::TextScreen;

namespace {

std::string row_text(const TextScreen& screen, int row, int from, int count) {
    std::string text;
    for (int column = from; column < from + count; ++column) {
        text.push_back(static_cast<char>(screen.cell(row, column).character));
    }
    return text;
}

bool type(LineEditor& editor, const std::string& keys) {
    bool ended = false;
    for (const char c : keys) {
        ended = editor.key(Key{static_cast<std::uint8_t>(c), 0});
    }
    return ended;
}

void editing() {
    TextScreen screen;
    screen.locate(3, 5);
    LineEditor editor(screen);
    CHECK(!type(editor, "abc"));
    CHECK(editor.text() == "abc");
    CHECK(row_text(screen, 3, 5, 4) == "abc ");
    CHECK(screen.pos() == 8);
    CHECK(!editor.key(Key{8, 0}));
    CHECK(editor.text() == "ab");
    CHECK(row_text(screen, 3, 5, 3) == "ab ");
    CHECK(screen.pos() == 7);
    // Extended keys and control characters are ignored.
    CHECK(!editor.key(Key{0, 75}));
    CHECK(!editor.key(Key{9, 0}));
    CHECK(editor.text() == "ab");
    CHECK(!editor.key(Key{27, 0}));
    CHECK(editor.text().empty());
    CHECK(row_text(screen, 3, 5, 2) == "  ");
    CHECK(screen.pos() == 5);
    CHECK(!editor.key(Key{8, 0}));  // nothing left to erase
    CHECK(screen.pos() == 5);
    CHECK(!type(editor, "xy"));
    CHECK(editor.key(Key{13, 0}));
    CHECK(editor.text() == "xy");
    CHECK(screen.csrlin() == 3 && screen.pos() == 7);
}

void wrapping() {
    TextScreen screen;
    screen.locate(2, 79);
    LineEditor editor(screen);
    CHECK(!type(editor, "12"));
    CHECK(screen.csrlin() == 3 && screen.pos() == 1);
    CHECK(!editor.key(Key{8, 0}));
    CHECK(screen.csrlin() == 2 && screen.pos() == 80);
    CHECK(screen.cell(2, 80).character == ' ');
    CHECK(editor.text() == "1");
}

void limit() {
    TextScreen screen;
    LineEditor editor(screen);
    CHECK(!type(editor, std::string(300, 'z')));
    CHECK(editor.text().size() == LineEditor::kLimit);
}

bool same(const std::vector<InputField>& fields,
          const std::vector<InputField>& expected) {
    if (fields.size() != expected.size()) {
        return false;
    }
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (fields[i].text != expected[i].text ||
            fields[i].quoted != expected[i].quoted) {
            return false;
        }
    }
    return true;
}

void fields() {
    CHECK(same(*split_input(""), {{"", false}}));
    CHECK(same(*split_input("  12 ,abc  , 3"),
               {{"12", false}, {"abc", false}, {"3", false}}));
    CHECK(
        same(*split_input("\" a, b \" , x"), {{" a, b ", true}, {"x", false}}));
    CHECK(same(*split_input("a,"), {{"a", false}, {"", false}}));
    CHECK(same(*split_input("\"open"), {{"open", true}}));
    CHECK(!split_input("\"a\" b"));
}

void numbers() {
    CHECK(parse_input_number("") == 0.0);
    CHECK(parse_input_number("  42 ") == 42.0);
    CHECK(parse_input_number("-3.5") == -3.5);
    CHECK(parse_input_number("+.25") == 0.25);
    CHECK(parse_input_number("1E3") == 1000.0);
    CHECK(parse_input_number("2.5d-1") == 0.25);
    CHECK(parse_input_number("&H1F") == 31.0);
    CHECK(parse_input_number("&O17") == 15.0);
    CHECK(parse_input_number("&17") == 15.0);
    CHECK(parse_input_number("7%") == 7.0);
    CHECK(!parse_input_number("abc"));
    CHECK(!parse_input_number("1 2"));
    CHECK(!parse_input_number("."));
    CHECK(!parse_input_number("1e"));
    CHECK(!parse_input_number("&H"));
}

}  // namespace

int main() {
    editing();
    wrapping();
    limit();
    fields();
    numbers();
    return wfc::brun71::test::finish("input");
}
