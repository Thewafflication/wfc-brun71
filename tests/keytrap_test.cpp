// TC-0010: key trapping (tests/tc-0010-key-traps.tex).
#include <string>

#include "check.hpp"
#include "wfc/brun71/keytrap.hpp"

using wfc::brun71::Key;
using wfc::brun71::KeyTraps;
using wfc::brun71::TrapState;

namespace {

constexpr Key kF1{0, 59};
constexpr Key kF10{0, 68};
constexpr Key kUp{0, 72};
constexpr Key kF12{0, 134};

std::string code(int flags, int scan) {
    return std::string{static_cast<char>(flags), static_cast<char>(scan)};
}

void numbers() {
    CHECK(KeyTraps::valid(1) && KeyTraps::valid(25) && KeyTraps::valid(30) &&
          KeyTraps::valid(31));
    CHECK(!KeyTraps::valid(0) && !KeyTraps::valid(26) && !KeyTraps::valid(29) &&
          !KeyTraps::valid(32));
    KeyTraps traps;
    CHECK_ERROR(5, traps.set(0, TrapState::on));
    CHECK_ERROR(5, traps.set(27, TrapState::on));
    CHECK_ERROR(5, traps.handler(32, true));
    CHECK_ERROR(5, traps.define(14, code(0, 16)));
    CHECK_ERROR(5, traps.define(15, "x"));
}

void events() {
    KeyTraps traps;
    CHECK(!traps.armed());
    // No handler, or the trap off: the key stays with the program.
    traps.set(1, TrapState::on);
    CHECK(!traps.armed());
    CHECK(!traps.take(kF1));
    traps.handler(1, true);
    traps.set(1, TrapState::off);
    CHECK(!traps.take(kF1));
    traps.set(1, TrapState::on);
    CHECK(traps.armed());
    CHECK(!traps.take(Key{'a', 0}));
    CHECK(!traps.take(kF10));
    CHECK(traps.take(kF1));
    CHECK(traps.next() == 1);
    CHECK(!traps.next());
    // The handler stops the key; RETURN turns it on again.
    traps.begin(1);
    CHECK(traps.state(1) == TrapState::stopped);
    CHECK(traps.take(kF1));  // taken, waits
    CHECK(!traps.next());
    traps.end(1);
    CHECK(traps.state(1) == TrapState::on);
    CHECK(traps.next() == 1);
    // A handler that turns the key off keeps it off.
    traps.begin(1);
    traps.set(1, TrapState::off);
    traps.end(1);
    CHECK(traps.state(1) == TrapState::off);
}

void stopping() {
    KeyTraps traps;
    traps.handler(11, true);
    traps.handler(31, true);
    traps.set(11, TrapState::stopped);
    traps.set(31, TrapState::on);
    CHECK(traps.take(kUp));
    CHECK(traps.take(kF12));
    CHECK(traps.next() == 31);
    CHECK(!traps.next());
    traps.set(11, TrapState::on);
    CHECK(traps.next() == 11);
    // OFF forgets a waiting event.
    traps.set(11, TrapState::stopped);
    CHECK(traps.take(kUp));
    traps.set(11, TrapState::off);
    traps.set(11, TrapState::on);
    CHECK(!traps.next());
}

void user_keys() {
    KeyTraps traps;
    for (int key = 15; key <= 19; ++key) {
        traps.handler(key, true);
        traps.set(key, TrapState::on);
    }
    traps.define(15, code(0, 16));         // q
    traps.define(16, code(1, 16));         // Shift+Q
    traps.define(17, code(4, 30));         // Ctrl+A
    traps.define(18, code(8, 46));         // Alt+C
    traps.define(19, code(64 + 128, 82));  // extended Ins, Caps Lock ignored
    CHECK(traps.take(Key{'q', 0}));
    CHECK(traps.next() == 15);
    CHECK(traps.take(Key{'Q', 0}));
    CHECK(traps.next() == 16);
    CHECK(traps.take(Key{1, 0}));
    CHECK(traps.next() == 17);
    CHECK(traps.take(Key{0, 46}));
    CHECK(traps.next() == 18);
    CHECK(traps.take(Key{0, 82}));
    CHECK(traps.next() == 19);
    CHECK(!traps.take(Key{'w', 0}));
    CHECK(!traps.take(Key{'c', 0}));
}

}  // namespace

int main() {
    numbers();
    events();
    stopping();
    user_keys();
    return wfc::brun71::test::finish("key traps");
}
