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
| `WIDTH columns, rows` | done, in WFC | 40/80 by 25/43/50; clears, resets the viewport |
| `COLOR fg, bg, border` | done, in WFC | `SCREEN 0` ranges: fg 0-31, bg 0-7, border 0-15 |
| `LOCATE row, col, cursor, start, stop` | done, in WFC | |
| `CLS [0 \| 1 \| 2]` | done, in WFC | |
| `VIEW PRINT [top TO bottom]` | done, in WFC | |
| `PRINT` (layout) | done, in WFC | WFC formats the numbers (VB6 rules: a sign place and a trailing space) |
| `TAB(n)`, `SPC(n)` | done, in WFC | |
| `CSRLIN`, `POS(0)` | done, in WFC | |
| `SCREEN(row, col [, flag])` | library only | WFC: the name is the VB6 `Screen` object |
| `INKEY$` | done, in WFC | from the console, or the standard input when the output is redirected |
| `INPUT`, `LINE INPUT` | done, in WFC | the line editor: characters, Backspace, Esc, Enter |
| `INPUT$(n)` | done, in WFC | from the keyboard; `INPUT$(n, #f)` reads a file as in VB6 |
| `PRINT USING`, `PRINT #n, USING` | done, in WFC | |
| `WRITE` to the screen | planned | M2 |
| `BEEP`, `SOUND`, `PLAY`, `SLEEP` | done, in WFC | heard on the console's speaker when the screen is shown there |
| `KEY n`, `KEY ON`, `KEY OFF`, `KEY LIST` | done, in WFC | |
| `KEY(n) ON/OFF/STOP`, `ON KEY(n) GOSUB` | done, in WFC | interpreted only; checked at the module level |
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
| `INPUT` adds "? " after a prompt followed by `;` and when there is no prompt, none after a prompt followed by `,`; `LINE INPUT` never adds it | documented |
| The INPUT line editor takes characters from 32 up, at most 255; Backspace erases the last one (also across a wrapped line), Esc the whole line; the cursor keys, Tab and the other control keys do nothing | assumed (BRUN71 also edits with the cursor keys, Ins and Del) |
| `INPUT` asks again ("Redo from start" on a line of its own) when the number of fields is wrong, a numeric field is not a number or is out of the variable's range, a numeric field is quoted, or text follows a closing quote | documented ("Redo from start"); assumed (which cases) |
| An empty `INPUT` line gives every variable 0 or "" | assumed |
| Unquoted INPUT fields lose their surrounding blanks; quoted ones keep everything between the quotes; `LINE INPUT` keeps the whole line | documented |
| A numeric INPUT field takes a sign, decimals, an `E` or `D` exponent, `&H` and `&O` digits and one type suffix | assumed |
| `PRINT USING` rounds halves away from zero | assumed |
| `PRINT USING` without a sign field puts the minus of a negative number before the `$` of `$$`; it drops the 0 before the point when there is no room for it | assumed |
| A `PRINT USING` exponent field keeps one position for the sign when it has no sign field | documented (the manual's examples) |
| After the last value `PRINT USING` prints the format's text up to the next field | assumed |
| `PLAY`: N46 (A in O3) is 440 Hz; `MN` sounds 7/8 of a note, `MS` 3/4; each dot makes a note 1.5 times longer; `<` and `>` stop at octaves 0 and 6 | assumed |
| `BEEP` is 800 Hz for a quarter of a second | assumed |
| `SLEEP` ends at once when a key is already waiting; the key stays for `INKEY$` | assumed |
| A compiled program starts with empty soft-key texts | assumed |
| A trap with `KEY(n) ON` but no `ON KEY(n) GOSUB` leaves the key to the program | assumed |
| A trapped key ends `SLEEP` | documented |
| User-defined trap keys are matched by the character their scan code types on a US keyboard (letters, digits, Space, Enter, Esc, Tab, Backspace); Alt and extended keys by the scan code | assumed (BRUN71 reads the scan code from the keyboard) |
| The soft-key line: 8 columns a key, the number in normal video, 6 characters (5 for key 10) in reverse video; keys 11 and 12 are not shown | assumed |
| `PRINT USING` with a format that has no field is error 5; a string in a numeric field, or a number in a string field, error 13 | documented |

## WFC differences

- `ERROR$(5)` gives Visual Basic's "Invalid procedure call or argument"; the
  error raised by the screen carries "Illegal function call" as its
  description.
- `LOCATE (5), 1` (a first argument in parentheses) is read as an array
  element, as in Visual Basic, not as the statement.
- A program that has its own `Sub Locate` (or `Cls`, `Color`, `Width`) keeps
  calling it in the interpreter; compiled to native code it gets the
  statement.
- Key events are checked only before statements of the module level, not
  inside `SUB` and `FUNCTION` procedures (BRUN71, compiled with `/V`, checks
  everywhere); an event that comes in a procedure waits until it returns.
  Native code does not compile `GOSUB` yet, so a program with `ON KEY` runs
  in the interpreter.
- With keys from the standard input, the check before each statement reads
  one key when none is waiting, so the handlers run as early as the keys
  allow.
- `MB` (music in the background) plays in the foreground, like `MF`; the
  program waits for the music.
- A VB6 `Beep` in a program that has not started the text screen stays
  silent, as before.
- A variable without a type suffix or `DEF` type is a Variant in WFC, not a
  single-precision number: `INPUT n` stores a number when the field is one
  and the text otherwise, instead of asking again.
- An INPUT field typed into a `Currency` or `Decimal` variable is converted
  as Visual Basic converts a number; one out of range raises error 6 instead
  of asking again.

## Host differences

- A console has no blink: the blink bit (`COLOR 16`-`31`) is dropped and the
  colours are kept.
- `SOUND` and `PLAY` use the Windows `Beep` (the speaker or the default
  sound device); with the output redirected nothing is heard and no time
  passes.
- A console has no border: `COLOR , , border` is kept but not shown.
- The console may refuse a size (a window smaller than 80 by 25); the cells are
  written anyway and the console clips them.
