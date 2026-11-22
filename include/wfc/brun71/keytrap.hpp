// Key trapping of BASIC PDS 7.1: ON KEY(n) GOSUB, KEY(n) ON/OFF/STOP, and
// the user-defined keys of KEY n, CHR$(flags) + CHR$(scancode).
//
// The keys: 1-10 F1-F10, 11 Up, 12 Left, 13 Right, 14 Down, 15-25 defined by
// the program, 30 F11, 31 F12. A key that is trapped (ON or STOP, with a
// handler) is taken from the keyboard before INKEY$ or INPUT see it; with ON
// its event fires at the next check, with STOP it waits for KEY(n) ON. The
// host runs the handler between statements, calling begin() before it and
// end() at its RETURN: during the handler the key is stopped, and RETURN
// turns it on again unless the handler turned it off.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>

#include "wfc/brun71/keyboard.hpp"

namespace wfc::brun71 {

enum class TrapState : std::uint8_t { off, on, stopped };

class KeyTraps {
public:
    // The highest trap number (F12).
    static constexpr int kLast = 31;

    // True for 1-25, 30 and 31.
    static bool valid(int key) noexcept;

    // KEY(n) ON/OFF/STOP; error 5 for a key that is not valid. OFF forgets an
    // event that was waiting.
    void set(int key, TrapState state);
    TrapState state(int key) const;

    // ON KEY(n) GOSUB label (true) or GOSUB 0 (false); error 5 as set().
    void handler(int key, bool present);

    // KEY n, CHR$(flags) + CHR$(scancode) for n = 15-25; error 5 for another
    // n or a text that is not two characters. Flags: 1-3 Shift, 4 Ctrl,
    // 8 Alt, 128 an extended key (32 Num Lock and 64 Caps Lock are ignored).
    void define(int key, std::string_view code);

    // Takes the key when a trap catches it (the event is then waiting);
    // false leaves it to the program.
    bool take(Key key);

    // The next event to run: a waiting event of a key that is on, with a
    // handler (the lowest number first); it is no longer waiting.
    std::optional<int> next();

    // Around the handler of an event: begin() stops the key, end() (its
    // RETURN) turns it on again if it is still stopped by begin().
    void begin(int key);
    void end(int key);

    // Some trap is on or stopped with a handler: the host checks for events.
    bool armed() const noexcept;

private:
    struct Trap {
        TrapState state = TrapState::off;
        bool handler = false;
        bool waiting = false;
        bool stopped_by_event = false;
        // Keys 15-25: the flags and scan code, 0 0 when not defined.
        std::uint8_t flags = 0;
        std::uint8_t scan = 0;
    };

    bool matches(int number, Key key) const;
    Trap& at(int key);
    const Trap& at(int key) const;

    std::array<Trap, kLast + 1> traps_{};
};

}  // namespace wfc::brun71
