// TC-0003: code page 437 (tests/tc-0003-code-page.tex).
#include "wfc/brun71/cp437.hpp"
#include "check.hpp"

using wfc::brun71::cp437_to_unicode;
using wfc::brun71::unicode_to_cp437;

int main() {
    // Every code but 0 (shown blank) comes back from its character.
    for (int code = 1; code < 256; ++code) {
        const char32_t shown =
            cp437_to_unicode(static_cast<std::uint8_t>(code));
        const auto back = unicode_to_cp437(shown);
        CHECK(back && *back == code);
        for (int other = 1; other < code; ++other) {
            CHECK(cp437_to_unicode(static_cast<std::uint8_t>(other)) != shown);
        }
    }
    CHECK(cp437_to_unicode(0) == U' ');
    CHECK(cp437_to_unicode(1) == char32_t{0x263A});
    CHECK(cp437_to_unicode('A') == U'A');
    CHECK(cp437_to_unicode(127) == char32_t{0x2302});
    CHECK(cp437_to_unicode(0x80) == char32_t{0x00C7});
    CHECK(cp437_to_unicode(0xB0) == char32_t{0x2591});
    CHECK(cp437_to_unicode(0xC9) == char32_t{0x2554});
    CHECK(cp437_to_unicode(0xDB) == char32_t{0x2588});
    CHECK(cp437_to_unicode(0xE1) == char32_t{0x00DF});
    CHECK(cp437_to_unicode(0xFF) == char32_t{0x00A0});

    // Control characters are typed as themselves.
    CHECK(unicode_to_cp437(U'\r') == std::uint8_t{13});
    CHECK(unicode_to_cp437(27) == std::uint8_t{27});
    CHECK(unicode_to_cp437(127) == std::uint8_t{127});
    CHECK(unicode_to_cp437(char32_t{0x00E9}) == std::uint8_t{130});
    CHECK(!unicode_to_cp437(char32_t{0x20AC}));
    CHECK(!unicode_to_cp437(U'\U0001F600'));
    return wfc::brun71::test::finish("cp437");
}
