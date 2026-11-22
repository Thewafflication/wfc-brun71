# Test cases of wfc-brun71

The corpus programs here are PDS 7.1 (QuickBASIC) programs that run on this
runtime through [WFC](https://github.com/Thewafflication/wfc): each
`NNN-name.bas` must print exactly the matching `.expected` text. A program
that uses the text screen prints, after any plain output, the text of its
final screen (rows 1 to the last one that is not blank). A `.keys` file is the
program's standard input: the keys it reads with `INKEY$` (each byte a key, a
line break is Enter, a 0 byte is followed by the scan code of an extended
key) and the lines it reads with `INPUT`.

WFC registers each program twice, `TC-MP0002-corpus-NNN-name` (interpreter)
and `TC-MP0002-corpus-NNN-name-native`. This repository runs them as
`TC-0005-NNN-name` and `TC-0005-NNN-name-native` when CMake is given
`-DWFC_EXECUTABLE=<wfc.exe>` (`run_corpus_case.cmake` is the runner); without
it they are skipped. The case is specified in `tests/tc-0005-pds-programs.tex`
and verifies `REQ-0004` to `REQ-0008` (see `requirements/`).

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
