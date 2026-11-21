// A minimal check for the test programs: CHECK prints a FAIL line with the
// place of the failed condition and the program exits 1 at the end;
// CHECK_ERROR checks that a statement raises the BASIC error given.
#pragma once

#include <cstdio>

#include "wfc/brun71/error.hpp"

namespace wfc::brun71::test {

inline int failures = 0;

inline void fail(const char* what, const char* file, int line) {
    std::printf("FAIL %s:%d: %s\n", file, line, what);
    ++failures;
}

inline int finish(const char* name) {
    if (failures != 0) {
        std::printf("%s: %d failure(s)\n", name, failures);
        return 1;
    }
    std::printf("%s: ok\n", name);
    return 0;
}

}  // namespace wfc::brun71::test

#define CHECK(condition)                                               \
    do {                                                               \
        if (!(condition)) {                                            \
            ::wfc::brun71::test::fail(#condition, __FILE__, __LINE__); \
        }                                                              \
    } while (false)

#define CHECK_ERROR(expected, statement)                                      \
    do {                                                                      \
        int raised_ = 0;                                                      \
        try {                                                                 \
            statement;                                                        \
        } catch (const ::wfc::brun71::BasicError& error_) {                   \
            raised_ = error_.number();                                        \
        }                                                                     \
        if (raised_ != (expected)) {                                          \
            ::wfc::brun71::test::fail("error " #expected " from " #statement, \
                                      __FILE__, __LINE__);                    \
        }                                                                     \
    } while (false)
