# REQ-0001 — Text screen of `SCREEN 0`

**Content type:** Project requirement

**Status:** Proposed (no BRUN71 reference probe; behaviour follows the PDS 7.1
and QuickBASIC 4.5 documentation, see `COMPATIBILITY.md`)

## Statement

- **Screen.** A grid of 40 or 80 columns by 25, 43 or 50 rows; each cell holds
  a code page 437 character and a CGA attribute (bits 0-3 foreground, 4-6
  background, 7 blink). A new screen is 80 by 25, blank, attribute 7, with the
  cursor hidden at row 1, column 1. Rows and columns are 1-based.
- **WIDTH.** `WIDTH columns, rows` accepts 40 or 80 columns and 25, 43 or 50
  rows, keeps an omitted value, clears the screen, resets the viewport and
  homes the cursor.
- **COLOR.** Foreground 0-31 (16-31 set the blink bit), background 0-7,
  border 0-15; an omitted value is kept; later output takes the attribute.
- **LOCATE.** Row within the viewport or the bottom line of the screen, column
  within the width, cursor 0 or 1, start and stop scan lines 0-31 (stop
  defaults to start); omitted values are kept.
- **VIEW PRINT.** `top TO bottom` within the screen with top not below bottom
  sets the viewport; with neither, the viewport is the whole screen. The
  cursor goes to the top line of the viewport. A program starts, and `WIDTH`
  leaves it, with the viewport on every line but the bottom one.
- **CLS.** No argument or 2 clears the viewport, 0 or 1 the whole screen,
  with the current attribute; the cursor goes to the top line of the
  viewport, column 1.
- **PRINT.** Characters are written at the cursor with the current attribute;
  writing the last column wraps to the next line; a new line from the bottom
  line of the viewport, or from below it, scrolls the viewport up one line
  and leaves the cursor on its bottom line. An item that does not fit on the
  rest of the line but fits on a line starts on the next line. Codes 7 (bell),
  9 (tab to the next multiple of 8), 10 and 13 (new line), 11 (home), 12
  (clear the viewport), 28-31 (cursor right, left, up, down) act instead of
  printing; other codes print their glyphs. A comma moves to the next
  14-column zone, or the next line when none is left. `TAB(n)` moves to column
  `n` (modulo the width, at least 1), on the next line when the cursor is past
  it; `SPC(n)` prints `n` modulo the width spaces.
- **Functions.** `CSRLIN` and `POS` give the cursor; `SCREEN(row, column)`
  the character code of a cell, and with a non-zero third argument its
  attribute.
- **Errors.** An argument out of range raises error 5 ("Illegal function
  call") and changes nothing.

## Verification

**Method:** Test

**References:** `TC-0001`
