#include "wfc/brun71/forms.hpp"

#include <algorithm>

namespace wfc::brun71 {
namespace {

// Code page 437 box drawing: corners top-left, top-right, bottom-left,
// bottom-right, then the horizontal and the vertical line.
struct BoxCharacters {
    std::uint8_t top_left;
    std::uint8_t top_right;
    std::uint8_t bottom_left;
    std::uint8_t bottom_right;
    std::uint8_t horizontal;
    std::uint8_t vertical;
};

constexpr BoxCharacters kSingle{0xDA, 0xBF, 0xC0, 0xD9, 0xC4, 0xB3};
constexpr BoxCharacters kDouble{0xC9, 0xBB, 0xC8, 0xBC, 0xCD, 0xBA};

bool on_screen(const TextScreen& screen, const int row, const int column) {
    return row >= 1 && row <= screen.rows() && column >= 1 &&
           column <= screen.columns();
}

}  // namespace

void put_clipped(TextScreen& screen, const int row, const int column,
                 const Cell cell) {
    if (on_screen(screen, row, column)) {
        screen.put(row, column, cell);
    }
}

void fill(TextScreen& screen, const Region& region,
          const std::uint8_t attribute, const std::uint8_t character) {
    const int top = std::max(region.top, 1);
    const int bottom = std::min(region.bottom(), screen.rows());
    const int left = std::max(region.left, 1);
    const int right = std::min(region.right(), screen.columns());
    for (int row = top; row <= bottom; ++row) {
        for (int column = left; column <= right; ++column) {
            screen.put(row, column, Cell{character, attribute});
        }
    }
}

void frame(TextScreen& screen, const Region& region, const Border border,
           const std::uint8_t attribute) {
    if (region.height < 2 || region.width < 2) {
        return;
    }
    const BoxCharacters& box = border == Border::single ? kSingle : kDouble;
    for (int column = region.left + 1; column < region.right(); ++column) {
        put_clipped(screen, region.top, column,
                    Cell{box.horizontal, attribute});
        put_clipped(screen, region.bottom(), column,
                    Cell{box.horizontal, attribute});
    }
    for (int row = region.top + 1; row < region.bottom(); ++row) {
        put_clipped(screen, row, region.left, Cell{box.vertical, attribute});
        put_clipped(screen, row, region.right(), Cell{box.vertical, attribute});
    }
    put_clipped(screen, region.top, region.left, Cell{box.top_left, attribute});
    put_clipped(screen, region.top, region.right(),
                Cell{box.top_right, attribute});
    put_clipped(screen, region.bottom(), region.left,
                Cell{box.bottom_left, attribute});
    put_clipped(screen, region.bottom(), region.right(),
                Cell{box.bottom_right, attribute});
}

void title(TextScreen& screen, const Region& region,
           const std::string_view text_bytes, const std::uint8_t attribute) {
    const int room = region.width - 2;
    if (text_bytes.empty() || room < 3) {
        return;
    }
    std::string padded = " ";
    padded += text_bytes;
    padded += ' ';
    if (static_cast<int>(padded.size()) > room) {
        padded.resize(static_cast<std::size_t>(room));
    }
    const int length = static_cast<int>(padded.size());
    const int column = region.left + 1 + (room - length) / 2;
    static_cast<void>(
        text(screen, region.top, column, padded, attribute, length));
}

void shadow(TextScreen& screen, const Region& region) {
    const auto darken = [&](const int row, const int column) {
        if (on_screen(screen, row, column)) {
            Cell cell = screen.cell(row, column);
            cell.attribute = kShadowAttribute;
            screen.put(row, column, cell);
        }
    };
    for (int row = region.top + 1; row <= region.bottom() + 1; ++row) {
        darken(row, region.right() + 1);
        darken(row, region.right() + 2);
    }
    for (int column = region.left + 2; column <= region.right(); ++column) {
        darken(region.bottom() + 1, column);
    }
}

int text(TextScreen& screen, const int row, const int column,
         const std::string_view bytes, const std::uint8_t attribute,
         const int width) {
    const int count = std::min(static_cast<int>(bytes.size()), width);
    for (int i = 0; i < count; ++i) {
        put_clipped(
            screen, row, column + i,
            Cell{static_cast<std::uint8_t>(bytes[static_cast<std::size_t>(i)]),
                 attribute});
    }
    return std::max(count, 0);
}

Caption caption(const std::string_view marked) {
    Caption result;
    for (std::size_t i = 0; i < marked.size(); ++i) {
        if (marked[i] == '&' && i + 1 < marked.size()) {
            ++i;
            if (marked[i] != '&' && result.access < 0) {
                result.access = static_cast<int>(result.text.size());
            }
        }
        result.text.push_back(marked[i]);
    }
    return result;
}

std::string button_face(const std::string_view text_bytes, const int width) {
    std::string face = "< ";
    face += text_bytes;
    face += " >";
    if (width <= 0) {
        return {};
    }
    if (static_cast<int>(face.size()) > width) {
        face.resize(static_cast<std::size_t>(width));
        return face;
    }
    const int before = (width - static_cast<int>(face.size())) / 2;
    return std::string(static_cast<std::size_t>(before), ' ') + face;
}

}  // namespace wfc::brun71
