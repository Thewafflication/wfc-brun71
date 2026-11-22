#include "wfc/brun71/mouse.hpp"

namespace wfc::brun71 {

std::vector<MouseEvent> mouse_changes(const int before, const int after,
                                      const bool moved, const bool double_click,
                                      const int row, const int column) {
    std::vector<MouseEvent> events;
    const auto add = [&](const MouseEvent::Kind kind, const int buttons) {
        MouseEvent event;
        event.kind = kind;
        event.buttons = buttons;
        event.row = row;
        event.column = column;
        events.push_back(event);
    };
    bool changed = false;
    for (const int button : {kLeftButton, kRightButton, kMiddleButton}) {
        const bool was = (before & button) != 0;
        const bool is = (after & button) != 0;
        if (!was && is) {
            add(double_click && button == kLeftButton
                    ? MouseEvent::Kind::double_click
                    : MouseEvent::Kind::down,
                button);
            changed = true;
        } else if (was && !is) {
            add(MouseEvent::Kind::up, button);
            changed = true;
        }
    }
    if (moved && !changed) {
        add(MouseEvent::Kind::move, after);
    }
    return events;
}

std::optional<MouseEvent> mouse_record(const std::string_view bytes) {
    if (bytes.size() != 4U) {
        return std::nullopt;
    }
    MouseEvent event;
    switch (bytes[0]) {
        case 'D':
            event.kind = MouseEvent::Kind::down;
            break;
        case 'U':
            event.kind = MouseEvent::Kind::up;
            break;
        case 'M':
            event.kind = MouseEvent::Kind::move;
            break;
        case '2':
            event.kind = MouseEvent::Kind::double_click;
            break;
        default:
            return std::nullopt;
    }
    event.buttons = static_cast<unsigned char>(bytes[1]);
    event.row = static_cast<unsigned char>(bytes[2]);
    event.column = static_cast<unsigned char>(bytes[3]);
    if (event.row < 1 || event.column < 1) {
        return std::nullopt;
    }
    return event;
}

}  // namespace wfc::brun71
