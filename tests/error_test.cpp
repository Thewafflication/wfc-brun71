// TC-0011: the run-time error messages (tests/tc-0011-errors.tex).
#include <cstring>

#include "check.hpp"
#include "wfc/brun71/error.hpp"

using wfc::brun71::basic_error_text;
using wfc::brun71::BasicError;
using wfc::brun71::error_text;

namespace {

bool is(const char* text, const char* expected) {
    return text != nullptr && std::strcmp(text, expected) == 0;
}

void messages() {
    CHECK(is(basic_error_text(3), "RETURN without GOSUB"));
    CHECK(is(basic_error_text(5), "Illegal function call"));
    CHECK(is(basic_error_text(11), "Division by zero"));
    CHECK(is(basic_error_text(16), "String formula too complex"));
    CHECK(is(basic_error_text(52), "Bad file name or number"));
    CHECK(is(basic_error_text(62), "Input past end of file"));
    CHECK(is(basic_error_text(75), "Path/File access error"));
    CHECK(is(basic_error_text(76), "Path not found"));
    // Numbers BASIC does not define.
    CHECK(basic_error_text(0) == nullptr);
    CHECK(basic_error_text(1) == nullptr);
    CHECK(basic_error_text(8) == nullptr);
    CHECK(basic_error_text(77) == nullptr);
    CHECK(basic_error_text(1000) == nullptr);
    CHECK(is(error_text(8), "Unprintable error"));
    CHECK(is(error_text(13), "Type mismatch"));
}

void errors() {
    const BasicError error(5);
    CHECK(error.number() == 5);
    CHECK(is(error.what(), "Illegal function call"));
}

}  // namespace

int main() {
    messages();
    errors();
    return wfc::brun71::test::finish("errors");
}
