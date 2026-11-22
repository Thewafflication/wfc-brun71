// Key trapping (keytrap.hpp).
#include "wfc/brun71/keytrap.hpp"

#include <cstddef>

#include "wfc/brun71/error.hpp"

namespace wfc::brun71 {
namespace {

// The character a key of the US layout types: its scan code, without and
// with Shift; 0 0 for a key with no character here.
struct ScanCharacter {
    std::uint8_t scan;
    char plain;
    char shifted;
};

constexpr ScanCharacter kScanCharacters[] = {
    {1, 27, 27},    {2, '1', '!'},  {3, '2', '@'},  {4, '3', '#'},
    {5, '4', '$'},  {6, '5', '%'},  {7, '6', '^'},  {8, '7', '&'},
    {9, '8', '*'},  {10, '9', '('}, {11, '0', ')'}, {12, '-', '_'},
    {13, '=', '+'}, {14, 8, 8},     {15, 9, 0},     {16, 'q', 'Q'},
    {17, 'w', 'W'}, {18, 'e', 'E'}, {19, 'r', 'R'}, {20, 't', 'T'},
    {21, 'y', 'Y'}, {22, 'u', 'U'}, {23, 'i', 'I'}, {24, 'o', 'O'},
    {25, 'p', 'P'}, {28, 13, 13},   {30, 'a', 'A'}, {31, 's', 'S'},
    {32, 'd', 'D'}, {33, 'f', 'F'}, {34, 'g', 'G'}, {35, 'h', 'H'},
    {36, 'j', 'J'}, {37, 'k', 'K'}, {38, 'l', 'L'}, {44, 'z', 'Z'},
    {45, 'x', 'X'}, {46, 'c', 'C'}, {47, 'v', 'V'}, {48, 'b', 'B'},
    {49, 'n', 'N'}, {50, 'm', 'M'}, {57, ' ', ' '},
};

const ScanCharacter* character_of(const std::uint8_t scan) {
    for (const auto& entry : kScanCharacters) {
        if (entry.scan == scan) {
            return &entry;
        }
    }
    return nullptr;
}

// The extended code of the fixed trap keys (1-14, 30, 31), 0 for the others.
std::uint8_t fixed_code(const int key) {
    if (key >= 1 && key <= 10) {
        return static_cast<std::uint8_t>(58 + key);  // F1 59 ... F10 68
    }
    switch (key) {
        case 11:
            return 72;
        case 12:
            return 75;
        case 13:
            return 77;
        case 14:
            return 80;
        case 30:
            return 133;
        case 31:
            return 134;
        default:
            return 0;
    }
}

[[noreturn]] void refuse() {
    throw BasicError(kIllegalFunctionCall);
}

}  // namespace

bool KeyTraps::valid(const int key) noexcept {
    return (key >= 1 && key <= 25) || key == 30 || key == 31;
}

KeyTraps::Trap& KeyTraps::at(const int key) {
    if (!valid(key)) {
        refuse();
    }
    return traps_[static_cast<std::size_t>(key)];
}

const KeyTraps::Trap& KeyTraps::at(const int key) const {
    if (!valid(key)) {
        refuse();
    }
    return traps_[static_cast<std::size_t>(key)];
}

void KeyTraps::set(const int key, const TrapState state) {
    Trap& trap = at(key);
    trap.state = state;
    trap.stopped_by_event = false;
    if (state == TrapState::off) {
        trap.waiting = false;
    }
}

TrapState KeyTraps::state(const int key) const {
    return at(key).state;
}

void KeyTraps::handler(const int key, const bool present) {
    at(key).handler = present;
}

void KeyTraps::define(const int key, const std::string_view code) {
    if (key < 15 || key > 25 || code.size() != 2U) {
        refuse();
    }
    Trap& trap = at(key);
    trap.flags = static_cast<std::uint8_t>(code[0]);
    trap.scan = static_cast<std::uint8_t>(code[1]);
}

bool KeyTraps::matches(const int number, const Key key) const {
    const Trap& trap = traps_[static_cast<std::size_t>(number)];
    if (const std::uint8_t code = fixed_code(number); code != 0) {
        return key.extended() && key.code == code;
    }
    if (trap.scan == 0) {
        return false;
    }
    const int flags = trap.flags & ~(32 | 64);
    const auto* entry = character_of(trap.scan);
    if ((flags & (8 | 128)) != 0 || entry == nullptr) {
        // Alt+key and the extended keys report the scan code.
        return key.extended() && key.code == trap.scan;
    }
    if (key.extended()) {
        return false;
    }
    if ((flags & 4) != 0) {
        // Ctrl+letter: codes 1-26.
        const char letter = entry->shifted;
        return letter >= 'A' && letter <= 'Z' &&
               key.character == static_cast<std::uint8_t>(letter - 'A' + 1);
    }
    const char wanted = (flags & 3) != 0 ? entry->shifted : entry->plain;
    return wanted != 0 && key.character == static_cast<std::uint8_t>(wanted);
}

bool KeyTraps::take(const Key key) {
    for (int number = 1; number <= kLast; ++number) {
        if (!valid(number)) {
            continue;
        }
        Trap& trap = traps_[static_cast<std::size_t>(number)];
        if (trap.state == TrapState::off || !trap.handler ||
            !matches(number, key)) {
            continue;
        }
        trap.waiting = true;
        return true;
    }
    return false;
}

std::optional<int> KeyTraps::next() {
    for (int number = 1; number <= kLast; ++number) {
        Trap& trap = traps_[static_cast<std::size_t>(number)];
        if (trap.waiting && trap.state == TrapState::on && trap.handler) {
            trap.waiting = false;
            return number;
        }
    }
    return std::nullopt;
}

void KeyTraps::begin(const int key) {
    Trap& trap = at(key);
    trap.state = TrapState::stopped;
    trap.stopped_by_event = true;
}

void KeyTraps::end(const int key) {
    Trap& trap = at(key);
    if (trap.stopped_by_event && trap.state == TrapState::stopped) {
        trap.state = TrapState::on;
    }
    trap.stopped_by_event = false;
}

bool KeyTraps::armed() const noexcept {
    for (const Trap& trap : traps_) {
        if (trap.state != TrapState::off && trap.handler) {
            return true;
        }
    }
    return false;
}

}  // namespace wfc::brun71
