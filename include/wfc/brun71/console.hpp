// The Windows console host of the text screen: shows a TextScreen in a
// console screen buffer of its own and reads the keys typed into the console
// into a KeyBuffer. Windows only.
//
// The CGA attribute bits are the console's (blue 1, green 2, red 4,
// intensity 8, the same four bits again for the background), so a cell's
// attribute is shown as it is; the blink bit has no console equivalent and is
// dropped.
#pragma once

#include <cstdint>
#include <memory>

#include "wfc/brun71/keyboard.hpp"
#include "wfc/brun71/screen.hpp"

namespace wfc::brun71 {

class Console {
public:
    // Opens a screen buffer of its own in the console of the process and,
    // when activate is true, shows it in place of the current one until the
    // Console is destroyed. Returns null when the process has no console.
    static std::unique_ptr<Console> open(bool activate = true);

    ~Console();
    Console(const Console&) = delete;
    Console& operator=(const Console&) = delete;

    // Shows every cell and the cursor of the screen; the buffer takes the
    // screen's size.
    void present(const TextScreen& screen);

    // Moves the keys waiting in the console input into the buffer; a key
    // that does not fit is lost and the console beeps. Returns how many keys
    // were added.
    int poll(KeyBuffer& buffer);

    // Waits up to the given time for console input; true when some arrived.
    bool wait(int milliseconds);

    // What the buffer shows at a cell (1-based), for tests.
    struct Shown {
        char32_t character = 0;
        std::uint16_t attribute = 0;
    };
    Shown shown(int row, int column) const;

    // The cursor of the buffer (1-based position), for tests.
    struct Cursor {
        int row = 0;
        int column = 0;
        bool visible = false;
        int size = 0;
    };
    Cursor cursor() const;

private:
    Console(void* buffer, void* previous, void* input);

    void* buffer_;
    void* previous_;
    void* input_;
    int columns_ = 0;
    int rows_ = 0;
};

}  // namespace wfc::brun71
