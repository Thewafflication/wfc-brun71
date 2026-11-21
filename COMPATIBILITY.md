# wfc-brun71: compatibility with BRUN71

Target: **BRUN71.EXE 7.1 (Microsoft BASIC Professional Development System
7.1)**. Library version: **0.7.1**.

Written from the documented behaviour of the PDS 7.1 and QuickBASIC 4.5
statements. **No rule has been checked against BRUN71 itself yet** (there is
no reference probe under DOS); the rules marked *documented* below follow the
manuals, and those marked *assumed* are the library's reading where the
manuals do not say.

## Statements and functions

| Item | Status | Notes |
| --- | --- | --- |
| `WIDTH columns, rows` | done | 40/80 by 25/43/50; clears, resets the viewport |
| `COLOR fg, bg, border` | done | `SCREEN 0` ranges: fg 0-31, bg 0-7, border 0-15 |
| `LOCATE row, col, cursor, start, stop` | done | |
| `CLS [0 \| 1 \| 2]` | done | |
| `VIEW PRINT [top TO bottom]` | done | |
| `PRINT` (layout) | done | the caller formats numbers |
| `TAB(n)`, `SPC(n)` | done | |
| `CSRLIN`, `POS(0)` | done | |
| `SCREEN(row, col [, flag])` | done | |
| `INKEY$` | done | key codes and buffer; the host fills the buffer |
| `PRINT USING`, `WRITE`, number format | planned | M2 |
| `INPUT`, `LINE INPUT`, `INPUT$` | planned | M2 |
| `BEEP`, `SOUND`, `PLAY`, `SLEEP` | planned | M2; `CHR$(7)` already calls the host's bell |
| `KEY` (soft keys, `ON KEY`) | planned | M2 |
| `SCREEN n` (graphics) | planned | later |
| User Interface Toolbox | planned | M3 |

## Rules not yet checked against BRUN71

| Rule | Source |
| --- | --- |
| A program starts with the viewport on lines 1 to 24; line 25 is outside it (the soft-key line) and is not scrolled or cleared by `CLS` | assumed (GW-BASIC and QuickBASIC behaviour) |
| `VIEW PRINT` with no lines makes the whole screen the viewport | documented |
| `LOCATE` to a line outside the viewport is error 5, except the bottom line of the screen | assumed |
| `CLS` with no argument and `CLS 2` clear the viewport; `CLS 0` and (in text mode) `CLS 1` the whole screen; the cursor goes to the top of the viewport | documented |
| Printing in the last column wraps at once, so the last cell of the viewport scrolls it | assumed |
| A `PRINT` item that does not fit on the rest of the line, but fits on a line, starts on the next line | assumed (GW-BASIC rule) |
| `PRINT` acts on `CHR$(7)`, `9`, `10`, `11`, `12`, `13`, `28`-`31`; tab stops every 8 columns | assumed |
| Print zones are 14 columns wide | documented |
| `TAB(n)` past the width wraps modulo the width; `SPC(n)` prints `n` modulo the width spaces | documented |
| The cursor is hidden until `LOCATE , , 1`; its default shape is scan lines 6-7 | assumed |
| `LOCATE , , , start` sets `stop` to `start` | documented |
| The grey cursor keys give the same codes as the keypad ones | assumed (the BIOS gives `E0h` prefixes; `INKEY$` reports `CHR$(0)`) |

## Host differences

- A console has no blink: the blink bit (`COLOR 16`-`31`) is dropped and the
  colours are kept.
- A console has no border: `COLOR , , border` is kept but not shown.
- The console may refuse a size (a window smaller than 80 by 25); the cells are
  written anyway and the console clips them.
