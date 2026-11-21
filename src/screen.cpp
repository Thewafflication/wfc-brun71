#include "wfc/brun71/screen.hpp"

#include "wfc/brun71/error.hpp"

namespace wfc::brun71 {
namespace {

constexpr int kZoneWidth = 14;
constexpr int kTabStop = 8;

[[noreturn]] void illegal() {
    throw BasicError(kIllegalFunctionCall);
}

void require(bool condition) {
    if (!condition) {
        illegal();
    }
}

bool in_range(int value, int low, int high) {
    return value >= low && value <= high;
}

}  // namespace

std::uint8_t make_attribute(int foreground, int background) {
    const int blink = (foreground & 16) != 0 ? 0x80 : 0;
    return static_cast<std::uint8_t>(blink | ((background & 7) << 4) |
                                     (foreground & 15));
}

TextScreen::TextScreen() {
    reset(80, 25);
}

void TextScreen::reset(int columns, int rows) {
    columns_ = columns;
    rows_ = rows;
    cells_.assign(
        static_cast<std::size_t>(columns_) * static_cast<std::size_t>(rows_),
        Cell{32, attribute_});
    view_top_ = 1;
    view_bottom_ = rows_ - 1;
    row_ = 1;
    column_ = 1;
}

Cell& TextScreen::at(int row, int column) {
    return cells_[static_cast<std::size_t>(row - 1) *
                      static_cast<std::size_t>(columns_) +
                  static_cast<std::size_t>(column - 1)];
}

const Cell& TextScreen::cell(int row, int column) const {
    require(in_range(row, 1, rows_) && in_range(column, 1, columns_));
    return cells_[static_cast<std::size_t>(row - 1) *
                      static_cast<std::size_t>(columns_) +
                  static_cast<std::size_t>(column - 1)];
}

void TextScreen::put(int row, int column, Cell value) {
    require(in_range(row, 1, rows_) && in_range(column, 1, columns_));
    at(row, column) = value;
}

void TextScreen::clear_rows(int top, int bottom) {
    for (int row = top; row <= bottom; ++row) {
        for (int column = 1; column <= columns_; ++column) {
            at(row, column) = Cell{32, attribute_};
        }
    }
}

void TextScreen::width(std::optional<int> columns, std::optional<int> rows) {
    const int new_columns = columns.value_or(columns_);
    const int new_rows = rows.value_or(rows_);
    require(new_columns == 40 || new_columns == 80);
    require(new_rows == 25 || new_rows == 43 || new_rows == 50);
    reset(new_columns, new_rows);
}

void TextScreen::color(std::optional<int> foreground,
                       std::optional<int> background,
                       std::optional<int> border) {
    require(!foreground || in_range(*foreground, 0, 31));
    require(!background || in_range(*background, 0, 7));
    require(!border || in_range(*border, 0, 15));
    foreground_ = foreground.value_or(foreground_);
    background_ = background.value_or(background_);
    border_ = border.value_or(border_);
    attribute_ = make_attribute(foreground_, background_);
}

void TextScreen::locate(std::optional<int> row, std::optional<int> column,
                        std::optional<int> cursor, std::optional<int> start,
                        std::optional<int> stop) {
    if (row) {
        require(in_range(*row, view_top_, view_bottom_) || *row == rows_);
    }
    require(!column || in_range(*column, 1, columns_));
    require(!cursor || in_range(*cursor, 0, 1));
    require(!start || in_range(*start, 0, 31));
    require(!stop || in_range(*stop, 0, 31));
    row_ = row.value_or(row_);
    column_ = column.value_or(column_);
    if (cursor) {
        cursor_visible_ = *cursor != 0;
    }
    if (start) {
        cursor_start_ = *start;
        cursor_stop_ = stop.value_or(*start);
    } else if (stop) {
        cursor_stop_ = *stop;
    }
}

void TextScreen::view_print(std::optional<int> top, std::optional<int> bottom) {
    if (!top && !bottom) {
        view_top_ = 1;
        view_bottom_ = rows_;
    } else {
        require(top && bottom);
        require(in_range(*top, 1, rows_) && in_range(*bottom, *top, rows_));
        view_top_ = *top;
        view_bottom_ = *bottom;
    }
    row_ = view_top_;
    column_ = 1;
}

void TextScreen::cls(std::optional<int> mode) {
    require(!mode || in_range(*mode, 0, 2));
    if (mode && *mode != 2) {
        clear_rows(1, rows_);
    } else {
        clear_rows(view_top_, view_bottom_);
    }
    row_ = view_top_;
    column_ = 1;
}

void TextScreen::scroll_view() {
    for (int row = view_top_; row < view_bottom_; ++row) {
        for (int column = 1; column <= columns_; ++column) {
            at(row, column) = at(row + 1, column);
        }
    }
    clear_rows(view_bottom_, view_bottom_);
}

void TextScreen::advance_line() {
    column_ = 1;
    if (row_ < view_bottom_) {
        ++row_;
    } else {
        scroll_view();
        row_ = view_bottom_;
    }
}

void TextScreen::write_glyph(std::uint8_t character) {
    at(row_, column_) = Cell{character, attribute_};
    ++column_;
    if (column_ > columns_) {
        advance_line();
    }
}

void TextScreen::control(std::uint8_t character) {
    switch (character) {
        case 7:
            if (on_bell) {
                on_bell();
            }
            break;
        case 9: {
            const int next = ((column_ - 1) / kTabStop + 1) * kTabStop + 1;
            if (next > columns_) {
                advance_line();
            } else {
                while (column_ < next) {
                    write_glyph(32);
                }
            }
            break;
        }
        case 10:
        case 13:
            advance_line();
            break;
        case 11:
            row_ = view_top_;
            column_ = 1;
            break;
        case 12:
            cls(2);
            break;
        case 28:
            if (column_ < columns_) {
                ++column_;
            }
            break;
        case 29:
            if (column_ > 1) {
                --column_;
            }
            break;
        case 30:
            if (row_ > view_top_) {
                --row_;
            }
            break;
        case 31:
            if (row_ < view_bottom_) {
                ++row_;
            }
            break;
        default:
            write_glyph(character);
            break;
    }
}

void TextScreen::print_text(std::string_view text) {
    for (const char c : text) {
        const auto character = static_cast<std::uint8_t>(c);
        switch (character) {
            case 7:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 28:
            case 29:
            case 30:
            case 31:
                control(character);
                break;
            default:
                write_glyph(character);
                break;
        }
    }
}

void TextScreen::print_item(std::string_view text) {
    const auto length = static_cast<int>(text.size());
    if (column_ > 1 && length <= columns_ && column_ - 1 + length > columns_) {
        advance_line();
    }
    print_text(text);
}

void TextScreen::new_line() {
    advance_line();
}

void TextScreen::comma() {
    const int next = ((column_ - 1) / kZoneWidth + 1) * kZoneWidth + 1;
    if (next > columns_) {
        advance_line();
        return;
    }
    while (column_ < next) {
        write_glyph(32);
    }
}

void TextScreen::tab(int column) {
    if (column > columns_) {
        column = (column - 1) % columns_ + 1;
    }
    if (column < 1) {
        column = 1;
    }
    if (column_ > column) {
        advance_line();
    }
    while (column_ < column) {
        write_glyph(32);
    }
}

void TextScreen::spc(int count) {
    if (count <= 0) {
        return;
    }
    count %= columns_;
    for (int i = 0; i < count; ++i) {
        write_glyph(32);
    }
}

int TextScreen::screen(int row, int column, int colorflag) const {
    const Cell& value = cell(row, column);
    return colorflag != 0 ? value.attribute : value.character;
}

}  // namespace wfc::brun71
