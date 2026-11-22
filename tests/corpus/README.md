# Test cases of wfc-brun71

The corpus programs here are PDS 7.1 (QuickBASIC) programs that run on this
runtime through [WFC](https://github.com/Thewafflication/wfc): each
`NNN-name.bas` must print exactly the matching `.expected` text. A program
that uses the text screen prints, after any plain output, the text of its
final screen (rows 1 to the last one that is not blank). A `.keys` file is the
program's standard input: the keys it reads with `INKEY$` (each byte a key, a
line break is Enter, a 0 byte is followed by the scan code of an extended
key) and the lines it reads with `INPUT`. A `NNN-name.vbp` beside it is a VB6
project with `WfcTextUI=1` (WFC ADR-0012) that runs instead: its forms are
shown on the screen and the keys drive them; the two bytes 0, 0 in its
`.keys` file let its timers' clock move on to the next one that is due,
and the bytes 0, 255 begin a mouse record (kind, buttons, row, column). A `.exitcode` file gives the exit
status the program must end with when it is not 0.

WFC registers each program twice, `TC-MP0002-corpus-NNN-name` (interpreter)
and `TC-MP0002-corpus-NNN-name-native`. This repository runs them as
`TC-0005-NNN-name` and `TC-0005-NNN-name-native` when CMake is given
`-DWFC_EXECUTABLE=<wfc.exe>` (`run_corpus_case.cmake` is the runner); without
it they are skipped. The case is specified in `tests/tc-0005-pds-programs.tex`
and verifies `REQ-0004` to `REQ-0011` (see `requirements/`).

| Program | What it checks |
| --- | --- |
| `300-pds-screen` | plain output before the screen, `LOCATE`, zones, `TAB`, `SPC`, `COLOR`, `CSRLIN`, `POS`, wrapping, `VIEW PRINT` |
| `301-pds-inkey` | `INKEY$`: a letter, an extended key, Enter, no key |
| `302-pds-errors` | error 5 from arguments out of range, trapped with `ON ERROR` |
| `303-pds-names` | a VB6 module that uses the words as names keeps its plain output |
| `304-pds-scroll` | scrolling, the bottom line outside the default viewport |
| `305-pds-input` | `INPUT` (prompts, "? ", Backspace, "Redo from start", `INPUT;`, quoted fields), `LINE INPUT`, `INPUT$` |
| `306-pds-using` | `PRINT USING` to the plain output, a file and the screen; errors 13 and 5 |
| `307-pds-sound-keys` | `SOUND`, `PLAY`, `BEEP`, `SLEEP` ended by a key, `KEY n`, `KEY ON`, `KEY LIST`, the key line after `CLS 0`, errors 5 and 13 |
| `308-pds-sleep-api` | a VB6 module that declares the Windows `Sleep` keeps calling it |
| `309-pds-on-key` | `ON KEY(n) GOSUB`, `KEY(n) ON/STOP/OFF`, a user-defined key, a handler inside a handler, errors 5 |
| `310-pds-write-screen` | `WRITE` to the plain output and the screen, `SCREEN(row, col [, flag])`, error 5 |
| `311-pds-error-exit` | an error no handler takes: BRUN71's message on the screen, exit status 1 (`311-pds-error-exit.exitcode`) |
| `321-text-input` | typing in text boxes: the caret, Delete, Backspace, scrolling, `MaxLength`, a `KeyPress` that changes `KeyAscii`, `Locked`; option buttons in a frame moved with the arrow keys |
| `322-text-lists` | a list box (Down, End, a letter, its scroll bar), a combo box chosen from its open list, a scroll bar (Right, Page Up), a timer ticked by the 0, 0 key |
| `323-text-combo-open` | the same form with the combo box's list left open over the other controls |
| `324-text-menus` | a menu bar, Alt and a title's key, an `InputBox` edited, a `Shortcut` (Ctrl+N) with a `MsgBox` answered No, F10 and Right, a checked item, a menu in a menu, a disabled item |
| `325-text-msgbox` | a `MsgBox` left open when the keys end: its lines, buttons and default button |
| `326-text-mouse` | the mouse: a button, a check box, a text box's caret, a list item, a double click, the right button on the form (`MouseDown` in twips), a press released away from its button |
| `320-text-form` | a VB6 form on the text screen: centred box, title, label, check box, buttons; Space, Enter on the `Default` button, Tab, Alt and an access key |
