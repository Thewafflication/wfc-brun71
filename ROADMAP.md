# wfc-brun71 roadmap

Library: **BRUN71**, the WFC clone of the BASIC PDS 7.1 text-mode runtime,
then of the PDS 7.1 User Interface Toolbox. Current version **0.7.1**,
targeting **BRUN71.EXE 7.1 (Microsoft BASIC Professional Development System
7.1)**.

A checked item is present, tested and passing; an unchecked one is planned.

## M1: the runtime core (0.7.1)

- [x] The text screen of `SCREEN 0` (`screen.hpp`): `WIDTH`, `COLOR`,
      `LOCATE`, `CLS`, `VIEW PRINT`, the `PRINT` rules (wrap, scroll, print
      zones, `TAB`, `SPC`, control characters), `CSRLIN`, `POS`,
      `SCREEN(row, column [, colorflag])`, error 5 for arguments out of range.
- [x] The keyboard (`keyboard.hpp`): `INKEY$` strings, BIOS codes of the
      extended keys with Shift, Ctrl and Alt, the 15-key buffer, the
      translation of Windows key presses.
- [x] Code page 437 (`cp437.hpp`).
- [x] The Windows console host (`console.hpp`): a screen buffer of its own,
      CGA attributes, the cursor and its shape, key input.
- [x] Standalone build, WSP baseline, CTest test cases `TC-0001` to `TC-0004`.
- [ ] CI on Windows x86, x64 and ARM64 (the workflow is in place).

## M2: PDS programs run in WFC

- [ ] WFC runs a PDS/QuickBASIC program (`.BAS` modules with no forms) as a
      text-mode program on this runtime, with the interpreter and the native
      compiler: the statements of M1 reach the library instead of the VB6
      `Form`/`Screen` objects whose names they share (`Cls`, `Print`,
      `Width`, `Screen`, `Color`).
- [ ] `PRINT` of numbers in the PDS format (a leading sign place and a
      trailing space), `PRINT USING`, `WRITE`.
- [ ] `INPUT`, `LINE INPUT` and the line editor they use; `INPUT$`.
- [ ] `BEEP`, `SOUND`, `PLAY`; `SLEEP`; `KEY ON/OFF/LIST` and the soft-key
      line; `ON KEY(n) GOSUB` and `KEY(n) ON/OFF/STOP`.
- [ ] Corpus programs in `tests/corpus`, run by WFC as the other libraries'
      are.

## M3: the User Interface Toolbox

A clone of the PDS 7.1 User Interface Toolbox (`GENERAL.BAS`, `MOUSE.BAS`,
`MENU.BAS`, `WINDOW.BAS`) callable from programs WFC runs, drawn on the M1
screen:

- [ ] Mouse (`MouseInit`, `MouseShow`, `MouseHide`, `MousePoll`, `MouseBorder`)
      over console mouse input.
- [ ] Pull-down menus (`MenuInit`, `MenuSet`, `MenuItemToggle`, `MenuEvent`,
      `MenuDo`, shortcut keys).
- [ ] Windows (`WindowInit`, `WindowOpen`, `WindowClose`, `WindowPrint`,
      `WindowDo`, buttons, edit fields, list boxes, `Alert`, `ListBox`).
- [ ] The toolbox's `GloStat`/`GloMenu`/`GloWindow` state and `TYPE`s as PDS
      declares them, so toolbox programs compile unchanged.

## Later

- [ ] Graphics screens (`SCREEN 1`, `2`, `7`-`13`) on a WFC window.
- [ ] Every statement of [COMPATIBILITY.md](COMPATIBILITY.md) checked against
      BRUN71 under DOS (a reference probe, as WFC does with VB6).

## Platform track

- [ ] Windows x86, x64, ARM64 (MSVC) passing in CI.
- [ ] Linux through Winelib; the screen and keyboard model on WebAssembly.

## WSP track

- [x] WSP 1.4.0 as a submodule (`wsp/`), adoption record in
      [docs/WSP-ADOPTION.md](docs/WSP-ADOPTION.md).
- [x] Requirements `REQ-0001` to `REQ-0003`, test cases `TC-0001` to
      `TC-0004`, traceability checked in the lint stage.
- [ ] Commit-checks record, static analysis, release records.
