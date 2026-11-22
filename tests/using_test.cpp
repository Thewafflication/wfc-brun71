// TC-0007: PRINT USING (tests/tc-0007-print-using.tex).
#include <string>
#include <vector>

#include "check.hpp"
#include "wfc/brun71/using.hpp"

using wfc::brun71::format_using;
using wfc::brun71::UsingValue;

namespace {

std::string with(const std::string& format, std::vector<UsingValue> values) {
    return format_using(format, values);
}

void digits() {
    CHECK(with("###", {5.0}) == "  5");
    CHECK(with("##.##", {3.14159}) == " 3.14");
    CHECK(with("##.##", {0.5}) == " 0.50");
    CHECK(with("#.##", {-0.5}) == "-.50");
    CHECK(with(".##", {0.5}) == ".50");
    CHECK(with("###", {-12.0}) == "-12");
    CHECK(with("###", {2.5}) == "  3");
    CHECK(with("#.#", {-0.01}) == "0.0");
    CHECK(with("##", {123.0}) == "%123");
    CHECK(with("#,###.##", {1234.5}) == "1,234.50");
    CHECK(with("##,###", {1234567.0}) == "%1,234,567");
}

void signs_and_fills() {
    CHECK(with("+###", {5.0}) == "  +5");
    CHECK(with("+###", {-5.0}) == "  -5");
    CHECK(with("###+", {5.0}) == "  5+");
    CHECK(with("###-", {-5.0}) == "  5-");
    CHECK(with("###-", {5.0}) == "  5 ");
    CHECK(with("**###", {12.0}) == "***12");
    CHECK(with("$$##.##", {5.5}) == "  $5.50");
    CHECK(with("**$##.##", {5.5}) == "***$5.50");
    CHECK(with("$$###", {-7.0}) == "  -$7");
}

void exponents() {
    CHECK(with("##.##^^^^", {234.56}) == " 2.35E+02");
    CHECK(with(".####^^^^-", {-777777.0}) == ".7778E+06-");
    CHECK(with("+.##^^^^", {123.0}) == "+.12E+03");
    CHECK(with("##.##^^^^^", {0.001}) == " 1.00E-003");
    CHECK(with("##.#^^^^", {9.96}) == " 1.0E+01");
    CHECK(with("##.##^^^^", {0.0}) == " 0.00E+00");
}

void strings() {
    CHECK(with("!", {std::string("hello")}) == "h");
    CHECK(with("!", {std::string()}) == " ");
    CHECK(with("\\  \\", {std::string("hello")}) == "hell");
    CHECK(with("\\  \\", {std::string("hi")}) == "hi  ");
    CHECK(with("&", {std::string("hello")}) == "hello");
    CHECK(with("[&]", {std::string("x")}) == "[x]");
}

void lists_and_literals() {
    CHECK(with("Total: ### units", {42.0}) == "Total:  42 units");
    CHECK(with("##", {1.0, 2.0, 3.0}) == " 1 2 3");
    CHECK(with("A## B##", {1.0}) == "A 1 B");
    CHECK(with("& is ##", {std::string("x"), 5.0, std::string("y"), 6.0}) ==
          "x is  5y is  6");
    CHECK(with("_###_!", {5.0}) == "# 5!");
    CHECK(with("$###", {5.0}) == "$  5");
    CHECK(with("no fields", {}) == "no fields");
    CHECK(with("\\x\\ #", {1.0}) == "\\x\\ 1");
    CHECK_ERROR(5, with("no fields", {1.0}));
    CHECK_ERROR(13, with("###", {std::string("a")}));
    CHECK_ERROR(13, with("&", {1.0}));
}

}  // namespace

int main() {
    digits();
    signs_and_fills();
    exponents();
    strings();
    lists_and_literals();
    return wfc::brun71::test::finish("print using");
}
