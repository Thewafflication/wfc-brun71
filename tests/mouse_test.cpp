// TC-0013: the mouse (tests/tc-0013-mouse.tex).
#include <string>

#include "check.hpp"
#include "wfc/brun71/mouse.hpp"

using wfc::brun71::kLeftButton;
using wfc::brun71::kMiddleButton;
using wfc::brun71::kRightButton;
using wfc::brun71::mouse_changes;
using wfc::brun71::mouse_record;
using Kind = wfc::brun71::MouseEvent::Kind;

namespace {

void changes() {
    auto events = mouse_changes(0, kLeftButton, false, false, 3, 7);
    CHECK(events.size() == 1U);
    CHECK(events[0].kind == Kind::down && events[0].buttons == kLeftButton);
    CHECK(events[0].row == 3 && events[0].column == 7);
    events = mouse_changes(kLeftButton, 0, false, false, 3, 7);
    CHECK(events.size() == 1U && events[0].kind == Kind::up);
    // A move with no button changed; a move that changed a button is only
    // the button's event.
    events = mouse_changes(kLeftButton, kLeftButton, true, false, 4, 7);
    CHECK(events.size() == 1U && events[0].kind == Kind::move);
    CHECK(events[0].buttons == kLeftButton);
    events = mouse_changes(0, kRightButton, true, false, 4, 7);
    CHECK(events.size() == 1U && events[0].kind == Kind::down);
    CHECK(events[0].buttons == kRightButton);
    // Two buttons at once: left first, then right; the middle one up.
    events = mouse_changes(kMiddleButton, kLeftButton | kRightButton, false,
                           false, 1, 1);
    CHECK(events.size() == 3U);
    CHECK(events[0].buttons == kLeftButton && events[0].kind == Kind::down);
    CHECK(events[1].buttons == kRightButton && events[1].kind == Kind::down);
    CHECK(events[2].buttons == kMiddleButton && events[2].kind == Kind::up);
    // A double click is the left button's; the right one stays a down.
    events = mouse_changes(0, kLeftButton, false, true, 2, 2);
    CHECK(events.size() == 1U && events[0].kind == Kind::double_click);
    events = mouse_changes(0, kRightButton, false, true, 2, 2);
    CHECK(events.size() == 1U && events[0].kind == Kind::down);
    CHECK(mouse_changes(0, 0, false, false, 1, 1).empty());
}

void records() {
    const auto down = mouse_record(std::string("D\x01\x05\x0A", 4));
    CHECK(down.has_value() && down->kind == Kind::down);
    CHECK(down->buttons == 1 && down->row == 5 && down->column == 10);
    CHECK(mouse_record("U\x02\x01\x01").value().kind == Kind::up);
    CHECK(mouse_record("M\x01\x01\x01").value().kind == Kind::move);
    CHECK(mouse_record("2\x01\x01\x01").value().kind == Kind::double_click);
    CHECK(!mouse_record("X\x01\x01\x01").has_value());
    CHECK(!mouse_record(std::string("D\x01\x00\x01", 4)).has_value());
    CHECK(!mouse_record("D\x01\x01").has_value());
}

}  // namespace

int main() {
    changes();
    records();
    return wfc::brun71::test::finish("mouse");
}
