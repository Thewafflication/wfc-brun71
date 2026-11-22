// The run-time errors of BASIC PDS 7.1. A statement of this library given an
// argument out of range raises error 5, "Illegal function call", as BRUN71
// does; PRINT USING raises 13, Type mismatch, and a keyboard that has ended
// 62, Input past end of file. The caller (the WFC evaluator or a compiled
// program) turns the number into a trappable BASIC error. error_text() has
// the messages BRUN71 gives for the run-time errors (the PDS 7.1 list of
// run-time error codes, ISAM errors excluded).
#pragma once

#include <stdexcept>
#include <string>

namespace wfc::brun71 {

inline constexpr int kIllegalFunctionCall = 5;
inline constexpr int kTypeMismatch = 13;
inline constexpr int kInputPastEnd = 62;

// The message of a BASIC run-time error, or nullptr for a number BASIC does
// not define.
inline const char* basic_error_text(int number) {
    switch (number) {
        case 2:
            return "Syntax error";
        case 3:
            return "RETURN without GOSUB";
        case 4:
            return "Out of DATA";
        case 5:
            return "Illegal function call";
        case 6:
            return "Overflow";
        case 7:
            return "Out of memory";
        case 9:
            return "Subscript out of range";
        case 10:
            return "Duplicate definition";
        case 11:
            return "Division by zero";
        case 13:
            return "Type mismatch";
        case 14:
            return "Out of string space";
        case 16:
            return "String formula too complex";
        case 19:
            return "No RESUME";
        case 20:
            return "RESUME without error";
        case 24:
            return "Device timeout";
        case 25:
            return "Device fault";
        case 27:
            return "Out of paper";
        case 39:
            return "CASE ELSE expected";
        case 40:
            return "Variable required";
        case 50:
            return "FIELD overflow";
        case 51:
            return "Internal error";
        case 52:
            return "Bad file name or number";
        case 53:
            return "File not found";
        case 54:
            return "Bad file mode";
        case 55:
            return "File already open";
        case 56:
            return "FIELD statement active";
        case 57:
            return "Device I/O error";
        case 58:
            return "File already exists";
        case 59:
            return "Bad record length";
        case 61:
            return "Disk full";
        case 62:
            return "Input past end of file";
        case 63:
            return "Bad record number";
        case 64:
            return "Bad file name";
        case 67:
            return "Too many files";
        case 68:
            return "Device unavailable";
        case 69:
            return "Communication-buffer overflow";
        case 70:
            return "Permission denied";
        case 71:
            return "Disk not ready";
        case 72:
            return "Disk-media error";
        case 73:
            return "Advanced feature unavailable";
        case 74:
            return "Rename across disks";
        case 75:
            return "Path/File access error";
        case 76:
            return "Path not found";
        default:
            return nullptr;
    }
}

// The message BASIC gives for an error number; "Unprintable error" for one
// it does not define.
inline const char* error_text(int number) {
    const char* text = basic_error_text(number);
    return text != nullptr ? text : "Unprintable error";
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
