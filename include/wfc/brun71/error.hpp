// The run-time errors the BASIC PDS 7.1 runtime raises from this library.
// A statement given an argument out of range raises error 5, "Illegal
// function call", as BRUN71 does; the caller (the WFC evaluator or a compiled
// program) turns the number into a trappable BASIC error.
#pragma once

#include <stdexcept>
#include <string>

namespace wfc::brun71 {

inline constexpr int kIllegalFunctionCall = 5;

// The message BASIC gives for an error number of this library.
inline const char* error_text(int number) {
    switch (number) {
        case kIllegalFunctionCall:
            return "Illegal function call";
        default:
            return "Unprintable error";
    }
}

class BasicError : public std::runtime_error {
public:
    explicit BasicError(int number)
        : std::runtime_error(error_text(number)), number_(number) {}

    int number() const noexcept { return number_; }

private:
    int number_;
};

}  // namespace wfc::brun71
