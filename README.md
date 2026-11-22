# wfc-brun71

The BASIC PDS 7.1 text-mode runtime for WFC: a clone of what `BRUN71.EXE`
gives a QuickBASIC or Microsoft BASIC Professional Development System 7.1
program on a PC text screen, running in a Windows console.

- `include/wfc/brun71/screen.hpp` the text screen of `SCREEN 0`: 40 or 80
  columns by 25, 43 or 50 lines of code page 437 characters with CGA
  attributes, and the statements and functions that work on it: `WIDTH`,
  `COLOR`, `LOCATE`, `CLS`, `VIEW PRINT`, the rules of `PRINT` (wrapping,
  scrolling, print zones, `TAB`, `SPC`, control characters), `CSRLIN`, `POS`
  and `SCREEN(row, column)`.
- `include/wfc/brun71/keyboard.hpp` the keyboard: the strings `INKEY$`
  returns (a character, or `CHR$(0)` and the BIOS scan code of an extended
  key, with the Shift, Ctrl and Alt codes), the 15-key type-ahead buffer, and
  the translation of Windows key presses.
- `include/wfc/brun71/input.hpp` the line editor of `INPUT` and
  `LINE INPUT`, and how `INPUT` splits its line into fields and numbers.
- `include/wfc/brun71/using.hpp` `PRINT USING`: numbers and strings
  formatted by the fields of a format string.
- `include/wfc/brun71/sound.hpp` `SOUND` and `PLAY`: the tones of the music
  macro language.
- `include/wfc/brun71/keyline.hpp` the soft keys: `KEY n`, `KEY ON`,
  `KEY OFF`, `KEY LIST`.
- `include/wfc/brun71/keytrap.hpp` key trapping: `ON KEY(n) GOSUB`,
  `KEY(n) ON/OFF/STOP` and the keys a program defines.
- `include/wfc/brun71/cp437.hpp` code page 437 to Unicode and back.
- `include/wfc/brun71/console.hpp` the Windows console host: shows a screen in
  a console screen buffer of its own and reads keys into the buffer.
- `include/wfc/brun71/error.hpp` the BASIC run-time errors and BRUN71's
  messages for them (the runtime raises 5, "Illegal function call", for an
  argument out of range; 13 and 62).

Target: `wfc_brun71` (static library, `wfcbrun71_static.lib`).

The model draws nothing itself, so the same rules serve the console host, the
tests and, later, other hosts. WFC runs PDS programs on it, interpreted and
as native code (WFC ADR-0011):

```
wfc game.bas              shows the screen live in the console
wfc game.bas < keys.txt   INKEY$ and INPUT read the keys from the file
wfc game.bas > out.txt    prints the text of the final screen
```

`tests/corpus` has the PDS programs WFC and this repository's `TC-0005` run. Next come VB6 forms in text mode: VB6
programs with an event-driven text user interface (forms, controls, menus,
the mouse) on the same screen. A clone of the PDS 7.1 User Interface Toolbox
comes later.

## Building on its own

```
git clone --recurse-submodules https://github.com/Thewafflication/wfc-brun71
cmake -S wfc-brun71 -B out/build
cmake --build out/build --config Debug
ctest --test-dir out/build -C Debug
```

## Roadmap, compatibility and platforms

- [ROADMAP.md](ROADMAP.md): milestones from the runtime core to VB6
  forms in text mode.
- [COMPATIBILITY.md](COMPATIBILITY.md): what is implemented, and which rules
  come from the documentation without a check against BRUN71 itself.
- [docs/WSP-ADOPTION.md](docs/WSP-ADOPTION.md): the WSP baseline the library
  follows (`wsp/` submodule).

| Platform | Status |
| --- | --- |
| Windows x64 (MSVC) | built and tested locally |
| Windows x86, ARM64 (MSVC) | CI workflow in place; first run pending |
| Linux through Winelib, WebAssembly | planned (the screen and keyboard model have no Windows dependency) |

## Version and target

The version is in `VERSION` and the Microsoft component it stands in for in
`TARGET`: **brun71** targets BRUN71.EXE 7.1 (Microsoft BASIC Professional
Development System 7.1). The version stays `0.<target major>.<target minor>`
(0.7.1) until the library matches the Microsoft one functionally.

Part of [WFC](https://github.com/Thewafflication/wfc), where it is mounted at
`lib/wfc-brun71`. Licence: GPL-3.0-or-later (see the WFC `LICENSE`).
