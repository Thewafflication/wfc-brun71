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

- [x] WFC runs a PDS/QuickBASIC program (`.BAS` modules with no forms) as a
      text-mode program on this runtime, with the interpreter and the native
      compiler (WFC ADR-0011): `CLS`, `LOCATE`, `COLOR`, `WIDTH`,
      `VIEW PRINT`, `INKEY$`, `CSRLIN`, `POS` in standard modules, and
      `PRINT` on the screen once it has started; live in the console, or
      the final screen as text when the output is redirected; keys from the
      console or the standard input (REQ-0004, TC-0005).
- [x] `SCREEN(row, col [, flag])` with its parentheses (`Screen` alone stays
      VB6's object); BRUN71's messages for an error no handler takes
      (REQ-0010, TC-0011).
- [x] `INPUT`, `LINE INPUT` from the keyboard and the line editor they use
      (`input.hpp`), "Redo from start"; `INPUT$(n)` (REQ-0005, TC-0006).
- [x] `PRINT USING` and `PRINT #n, USING` (`using.hpp`; REQ-0006,
      TC-0007).
- [x] `WRITE` to the screen and the plain output (REQ-0010).
- [x] `BEEP`, `SOUND`, `PLAY` (`sound.hpp`), `SLEEP` (REQ-0007, TC-0008);
      `KEY n`, `KEY ON/OFF/LIST` and the soft-key line (`keyline.hpp`;
      REQ-0008, TC-0009).
- [x] `ON KEY(n) GOSUB`, `KEY(n) ON/OFF/STOP` and the user-defined keys
      (`keytrap.hpp`; REQ-0009, TC-0010), in the interpreter.
- [x] Corpus programs in `tests/corpus`, run by WFC in the interpreter and
      as native code (`TC-MP0002-corpus-30x-*`), and here as `TC-0005`.

## M3: VB6 forms in text mode

The owner's direction (2026-11-22): write programs in the VB6 language with
an event-driven text user interface, as Visual Basic for MS-DOS did. A VB6
project whose forms are marked as text forms runs on the M1 screen instead
of Win32 windows; the program stays single-threaded and event-driven (events,
`Timer`, `DoEvents`), like VB6 itself.

- [x] Design record in WFC (ADR-0012): a text `UiHost` under the
      unchanged form model; `WfcTextUI=1` or `wfc --text-ui`; a cell is
      120 by 240 twips (`ScaleMode = 4`); which controls and events exist.
- [x] Step 1: the text host in WFC and the drawing in `forms.hpp`: forms
      as centred boxes with a caption and a shadow, the z-order, labels,
      command buttons, check and option boxes drawn; keys from the console
      or the standard input to the focused control, Tab, Enter, Esc, Space,
      Alt and an access key, Alt+F4 (REQ-0011, TC-0012, `320-text-form`).
- [x] Step 2: typing in text boxes (the caret, Backspace, Delete, Home,
      End, scrolling, `KeyPress` that changes `KeyAscii`, `Locked`,
      `MaxLength`), option buttons moved with the arrow keys, frames
      (`321-text-input`).
- [x] Step 3: list boxes and combo boxes (keys, a letter, scrolling, a
      scroll bar, the open list of a combo box), scroll bars, timers on the
      clock or, with keys from a file, on a virtual clock
      (`322-text-lists`, `323-text-combo-open`).
- [x] Step 4: the menu bar and pull-down menus (access keys, F10, check
      marks, menus in menus, `Shortcut`), `MsgBox` and `InputBox` as text
      dialogs (`324-text-menus`, `325-text-msgbox`).
- [x] Step 5: the console mouse (`mouse.hpp`; REQ-0011, TC-0013,
      `326-text-mouse`); `wfc build` writes a console program for a text
      form project (`TC-MP0007-package-320-text-form`).
- [x] WFC runs such a project in the interpreter and as native code;
      corpus programs drive it with `.keys` files and check the final
      screen.
- [ ] Modal forms, several forms at once and `DoEvents` checked in the
      corpus.
- [x] Checked in a live console window (conhost) with input written by a
      test driver: drawing and colours, keys, the mouse, real-time timers,
      Quick Edit off while running and the console restored after; a fast
      run found mouse events overtaking keys, now kept in order. Windows
      Terminal with a real mouse is not checked.

## Later

- [ ] Several threads or programs at once: isolated workers, each with its
      own interpreter, that pass messages to the program as events.
- [ ] The PDS 7.1 User Interface Toolbox (`GENERAL.BAS`, `MOUSE.BAS`,
      `MENU.BAS`, `WINDOW.BAS`), for old toolbox programs, drawn with the
      M3 form engine where it fits.
- [ ] Graphics screens (`SCREEN 1`, `2`, `7`-`13`) on a WFC window.
- [ ] Every statement of [COMPATIBILITY.md](COMPATIBILITY.md) checked against
      BRUN71 under DOS (a reference probe, as WFC does with VB6).

## Platform track

- [ ] Windows x86, x64, ARM64 (MSVC) passing in CI.
- [ ] Linux through Winelib; the screen and keyboard model on WebAssembly.

## WSP track

- [x] WSP 1.4.0 as a submodule (`wsp/`), adoption record in
      [docs/WSP-ADOPTION.md](docs/WSP-ADOPTION.md).
- [x] Requirements `REQ-0001` to `REQ-0011`, test cases `TC-0001` to
      `TC-0013`, traceability checked in the lint stage.
- [ ] Commit-checks record, static analysis, release records.
