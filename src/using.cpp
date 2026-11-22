// PRINT USING (using.hpp).
#include "wfc/brun71/using.hpp"

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <optional>

#include "wfc/brun71/error.hpp"

namespace wfc::brun71 {
namespace {

bool starts(const std::string_view format, const std::size_t at,
            const std::string_view text) {
    return format.substr(at, text.size()) == text;
}

// A numeric field of the format.
struct NumberField {
    std::size_t width = 0;     // characters of the format it takes
    int whole = 0;             // positions before the point (# , ** $$)
    int decimals = 0;          // # after the point
    bool point = false;        // has a decimal point
    bool commas = false;       // groups the whole part by thousands
    bool stars = false;        // ** fill
    bool dollar = false;       // $$ or **$
    bool lead_plus = false;    // + before
    bool trail_plus = false;   // + after
    bool trail_minus = false;  // - after
    int exponent = 0;          // 0, or 4 or 5 for ^^^^ and ^^^^^
};

// A numeric field starts here: # .# ** $$, or + before one of them.
bool number_starts(const std::string_view format, std::size_t at) {
    if (at < format.size() && format[at] == '+') {
        ++at;
    }
    if (at >= format.size()) {
        return false;
    }
    return format[at] == '#' || starts(format, at, ".#") ||
           starts(format, at, "**") || starts(format, at, "$$");
}

NumberField read_number_field(const std::string_view format,
                              const std::size_t start) {
    NumberField field;
    std::size_t at = start;
    if (format[at] == '+') {
        field.lead_plus = true;
        ++at;
    }
    if (starts(format, at, "**$")) {
        field.stars = true;
        field.dollar = true;
        field.whole = 3;
        at += 3U;
    } else if (starts(format, at, "**")) {
        field.stars = true;
        field.whole = 2;
        at += 2U;
    } else if (starts(format, at, "$$")) {
        field.dollar = true;
        field.whole = 2;
        at += 2U;
    }
    while (at < format.size() &&
           (format[at] == '#' || (format[at] == ',' && field.whole > 0))) {
        field.commas = field.commas || format[at] == ',';
        ++field.whole;
        ++at;
    }
    if (at < format.size() && format[at] == '.') {
        field.point = true;
        ++at;
        while (at < format.size() && format[at] == '#') {
            ++field.decimals;
            ++at;
        }
    }
    if (starts(format, at, "^^^^^")) {
        field.exponent = 5;
        at += 5U;
    } else if (starts(format, at, "^^^^")) {
        field.exponent = 4;
        at += 4U;
    }
    if (!field.lead_plus && at < format.size()) {
        if (format[at] == '+') {
            field.trail_plus = true;
            ++at;
        } else if (format[at] == '-') {
            field.trail_minus = true;
            ++at;
        }
    }
    field.width = at - start;
    return field;
}

// The number with the decimals given, halves rounded away from zero.
std::string fixed(double value, const int decimals) {
    const double scale = std::pow(10.0, decimals);
    if (std::fabs(value) * scale < 1e15) {
        value = std::round(value * scale) / scale;
    }
    char buffer[400];
    const int length =
        std::snprintf(buffer, sizeof buffer, "%.*f", decimals, value);
    return std::string(buffer,
                       length > 0 ? static_cast<std::size_t>(length) : 0U);
}

std::string group_thousands(const std::string& digits) {
    std::string grouped;
    const std::size_t size = digits.size();
    for (std::size_t i = 0; i < size; ++i) {
        if (i != 0 && (size - i) % 3U == 0U) {
            grouped.push_back(',');
        }
        grouped.push_back(digits[i]);
    }
    return grouped;
}

bool all_zero(const std::string& text) {
    for (const char c : text) {
        if (c >= '1' && c <= '9') {
            return false;
        }
    }
    return true;
}

// The digits of the number without its sign: whole part, point, decimals and
// the exponent.
std::string digits_of(const NumberField& field, const double magnitude,
                      bool& negative) {
    std::string whole;
    std::string decimals;
    std::string exponent;
    if (field.exponent != 0) {
        // The whole part keeps one position for the sign unless the field
        // has a sign of its own.
        const bool has_sign =
            field.lead_plus || field.trail_plus || field.trail_minus;
        int before = field.whole - (has_sign ? 0 : 1);
        if (before < 0) {
            before = 0;
        }
        int power = 0;
        double mantissa = 0.0;
        if (magnitude != 0.0) {
            power = static_cast<int>(std::floor(std::log10(magnitude))) + 1 -
                    before;
            mantissa = magnitude / std::pow(10.0, power);
            // Rounding may carry into one more digit (9.996 to 10.00).
            if (std::strtod(fixed(mantissa, field.decimals).c_str(), nullptr) >=
                std::pow(10.0, before)) {
                mantissa /= 10.0;
                ++power;
            }
        }
        const std::string text = fixed(mantissa, field.decimals);
        const auto point = text.find('.');
        whole = text.substr(0, point);
        if (point != std::string::npos) {
            decimals = text.substr(point + 1U);
        }
        if (before == 0) {
            whole.clear();
        }
        char buffer[16];
        std::snprintf(buffer, sizeof buffer, "%0*d", field.exponent - 2,
                      power < 0 ? -power : power);
        exponent = std::string("E") + (power < 0 ? "-" : "+") + buffer;
    } else {
        const std::string text = fixed(magnitude, field.decimals);
        const auto point = text.find('.');
        whole = text.substr(0, point);
        if (point != std::string::npos) {
            decimals = text.substr(point + 1U);
        }
        if (field.commas) {
            whole = group_thousands(whole);
        }
        if (whole == "0" && field.whole == 0) {
            whole.clear();
        }
    }
    if (negative && all_zero(whole + decimals)) {
        negative = false;
    }
    std::string text = whole;
    if (field.point) {
        text += "." + decimals;
    }
    return text + exponent;
}

std::string format_number(const NumberField& field, const double value) {
    bool negative = value < 0.0;
    std::string digits = digits_of(field, std::fabs(value), negative);
    std::string lead;
    std::string trail;
    if (field.lead_plus) {
        lead = negative ? "-" : "+";
    } else if (field.trail_plus) {
        trail = negative ? "-" : "+";
    } else if (field.trail_minus) {
        trail = negative ? "-" : " ";
    } else if (negative) {
        lead = "-";
    }
    if (field.dollar) {
        lead += "$";
    }
    std::string body = lead + digits + trail;
    if (body.size() > field.width && digits.size() > 1U &&
        digits.front() == '0' && digits[1] == '.') {
        // A lone zero before the point goes when there is no room for it.
        body = lead + digits.substr(1U) + trail;
    }
    if (body.size() > field.width) {
        return "%" + body;
    }
    return std::string(field.width - body.size(), field.stars ? '*' : ' ') +
           body;
}

// A string field at the position, or nullopt: ! & or \ blanks \.
std::optional<std::size_t> string_field(const std::string_view format,
                                        const std::size_t at,
                                        std::size_t& width) {
    if (format[at] == '!') {
        width = 1U;
        return 1U;
    }
    if (format[at] == '&') {
        width = 0U;  // the whole string
        return 1U;
    }
    if (format[at] == '\\') {
        std::size_t end = at + 1U;
        while (end < format.size() && format[end] == ' ') {
            ++end;
        }
        if (end < format.size() && format[end] == '\\') {
            width = end - at + 1U;
            return width;
        }
    }
    return std::nullopt;
}

}  // namespace

std::string format_using(const std::string_view format,
                         const std::span<const UsingValue> values) {
    std::string out;
    std::size_t next_value = 0;
    bool any_field = false;
    std::size_t at = 0;
    while (true) {
        if (at >= format.size()) {
            if (next_value >= values.size()) {
                return out;
            }
            if (!any_field) {
                throw BasicError(kIllegalFunctionCall);
            }
            at = 0;  // the format again from its start
            continue;
        }
        const char c = format[at];
        if (c == '_') {
            if (at + 1U < format.size()) {
                out.push_back(format[at + 1U]);
                at += 2U;
            } else {
                out.push_back('_');
                ++at;
            }
            continue;
        }
        std::size_t width = 0;
        if (const auto length = string_field(format, at, width)) {
            any_field = true;
            if (next_value >= values.size()) {
                return out;
            }
            const auto* text = std::get_if<std::string>(&values[next_value]);
            if (text == nullptr) {
                throw BasicError(kTypeMismatch);
            }
            if (width == 0U) {
                out += *text;
            } else {
                std::string shown = text->substr(0, width);
                shown.resize(width, ' ');
                out += shown;
            }
            ++next_value;
            at += *length;
            continue;
        }
        if (number_starts(format, at)) {
            any_field = true;
            if (next_value >= values.size()) {
                return out;
            }
            const auto* number = std::get_if<double>(&values[next_value]);
            if (number == nullptr) {
                throw BasicError(kTypeMismatch);
            }
            const NumberField field = read_number_field(format, at);
            out += format_number(field, *number);
            ++next_value;
            at += field.width;
            continue;
        }
        out.push_back(c);
        ++at;
    }
}

}  // namespace wfc::brun71
