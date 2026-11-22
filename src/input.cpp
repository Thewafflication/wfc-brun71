// The line editor of INPUT and how INPUT splits its line (input.hpp).
#include "wfc/brun71/input.hpp"

#include <cmath>
#include <cstdlib>

namespace wfc::brun71 {

bool LineEditor::key(const Key key) {
    if (key.extended()) {
        return false;
    }
    switch (key.character) {
        case 13:
            return true;
        case 8:
            erase_last();
            return false;
        case 27:
            while (!text_.empty()) {
                erase_last();
            }
            return false;
        default:
            break;
    }
    if (key.character < 32 || text_.size() >= kLimit) {
        return false;
    }
    text_.push_back(static_cast<char>(key.character));
    screen_.print_text(std::string_view(&text_.back(), 1U));
    return false;
}

void LineEditor::erase_last() {
    if (text_.empty()) {
        return;
    }
    text_.pop_back();
    int row = screen_.csrlin();
    int column = screen_.pos() - 1;
    if (column < 1) {
        // The character is at the end of the line above (a line that is full
        // wraps at once).
        if (row <= screen_.view_top()) {
            return;
        }
        --row;
        column = screen_.columns();
    }
    screen_.put(row, column, Cell{32, screen_.attribute()});
    screen_.locate(row, column);
}

std::optional<std::vector<InputField>> split_input(
    const std::string_view line) {
    std::vector<InputField> fields;
    std::size_t at = 0;
    const auto skip_blanks = [&] {
        while (at < line.size() && line[at] == ' ') {
            ++at;
        }
    };
    while (true) {
        skip_blanks();
        InputField field;
        if (at < line.size() && line[at] == '"') {
            field.quoted = true;
            const auto close = line.find('"', at + 1U);
            const auto end =
                close == std::string_view::npos ? line.size() : close;
            field.text = std::string(line.substr(at + 1U, end - at - 1U));
            at = close == std::string_view::npos ? line.size() : close + 1U;
            skip_blanks();
            if (at < line.size() && line[at] != ',') {
                return std::nullopt;
            }
        } else {
            const auto comma = line.find(',', at);
            const auto end =
                comma == std::string_view::npos ? line.size() : comma;
            auto text = line.substr(at, end - at);
            while (!text.empty() && text.back() == ' ') {
                text.remove_suffix(1U);
            }
            field.text = std::string(text);
            at = end;
        }
        fields.push_back(std::move(field));
        if (at >= line.size()) {
            return fields;
        }
        ++at;  // the comma
    }
}

namespace {

bool is_digit_of(const char c, const int base) {
    if (c >= '0' && c <= '9') {
        return c - '0' < base;
    }
    if (base == 16) {
        return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    }
    return false;
}

int digit_value(const char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    return (c | 0x20) - 'a' + 10;
}

}  // namespace

std::optional<double> parse_input_number(std::string_view field) {
    while (!field.empty() && field.front() == ' ') {
        field.remove_prefix(1U);
    }
    while (!field.empty() && field.back() == ' ') {
        field.remove_suffix(1U);
    }
    if (field.empty()) {
        return 0.0;
    }
    std::size_t at = 0;
    bool negative = false;
    if (field[at] == '+' || field[at] == '-') {
        negative = field[at] == '-';
        ++at;
    }
    double value = 0.0;
    if (at < field.size() && field[at] == '&') {
        ++at;
        int base = 8;
        if (at < field.size() && (field[at] == 'h' || field[at] == 'H')) {
            base = 16;
            ++at;
        } else if (at < field.size() &&
                   (field[at] == 'o' || field[at] == 'O')) {
            ++at;
        }
        const auto first = at;
        while (at < field.size() && is_digit_of(field[at], base)) {
            value = value * base + digit_value(field[at]);
            ++at;
        }
        if (at == first) {
            return std::nullopt;
        }
    } else {
        std::string spelling;
        std::size_t digits = 0;
        while (at < field.size() && field[at] >= '0' && field[at] <= '9') {
            spelling.push_back(field[at++]);
            ++digits;
        }
        if (at < field.size() && field[at] == '.') {
            spelling.push_back(field[at++]);
            while (at < field.size() && field[at] >= '0' && field[at] <= '9') {
                spelling.push_back(field[at++]);
                ++digits;
            }
        }
        if (digits == 0) {
            return std::nullopt;
        }
        if (at < field.size() && (field[at] == 'e' || field[at] == 'E' ||
                                  field[at] == 'd' || field[at] == 'D')) {
            std::string exponent = "e";
            std::size_t next = at + 1U;
            if (next < field.size() &&
                (field[next] == '+' || field[next] == '-')) {
                exponent.push_back(field[next++]);
            }
            std::size_t exponent_digits = 0;
            while (next < field.size() && field[next] >= '0' &&
                   field[next] <= '9') {
                exponent.push_back(field[next++]);
                ++exponent_digits;
            }
            if (exponent_digits == 0) {
                return std::nullopt;
            }
            spelling += exponent;
            at = next;
        }
        value = std::strtod(spelling.c_str(), nullptr);
    }
    if (at < field.size() && (field[at] == '!' || field[at] == '#' ||
                              field[at] == '%' || field[at] == '&')) {
        ++at;
    }
    if (at != field.size() || !std::isfinite(value)) {
        return std::nullopt;
    }
    return negative ? -value : value;
}

}  // namespace wfc::brun71
