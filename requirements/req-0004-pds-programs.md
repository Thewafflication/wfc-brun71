# REQ-0004 — PDS programs run in WFC

**Content type:** Project requirement

**Status:** Proposed (no BRUN71 reference probe; see `COMPATIBILITY.md` and
WFC `architecture/adr-0011-pds-text-programs.md`)

## Statement

- **Statements.** In a standard module, run by the interpreter or compiled to
  native code, `CLS [n]`, `LOCATE [row] [, [col] ...]`, `COLOR [fg] [, [bg]
  [, border]]`, `WIDTH [cols] [, rows]` and `VIEW PRINT [top TO bottom]` act on
  the text screen of REQ-0001 with their arguments rounded to whole numbers.
  A word followed by `=`, `.`, `#` or `(`, a procedure of the program with
  that name, and code of a form or control keep their Visual Basic meaning.
- **Functions.** `INKEY$`, `CSRLIN` and `POS(n)` (n ignored) return the next
  key (REQ-0002) and the cursor.
- **The session.** The screen starts with the first of these statements or
  functions; from then on `PRINT` to the console writes on it item by item
  (REQ-0001 layout rules, `TAB`, `SPC`, the 14-column zones), and output
  printed before it stays as plain output.
- **Console and redirected output.** When the standard output is a console,
  the screen is shown live in a console screen buffer of its own and keys
  come from the console. Otherwise keys come from the standard input (each
  byte a key, CR, LF or CR LF is Enter, a 0 byte is followed by an extended
  key's scan code); once it has ended, `INKEY$` keeps returning `""`, and after
  1,000,000 such calls raises error 62 instead of waiting for ever.
- **End.** When the program ends, the screen buffer goes away and the text of
  the final screen (rows 1 to the last one that is not blank, trailing blanks
  removed, code page 437 shown as Unicode) follows the plain output.
- **Errors.** Out-of-range arguments raise error 5 with the text "Illegal
  function call", trappable with `ON ERROR`; a String argument raises 13 and a
  number beyond -32768 to 32767 raises 6.

## Known limits

`SCREEN(row, col)` is not yet available (the name is VB6's `Screen` object);
`ERROR$(5)` gives Visual Basic's text; sound and `KEY` follow (ROADMAP M2).
`INPUT` and `PRINT USING` are REQ-0005 and REQ-0006.

## Verification

**Method:** Test

**References:** `TC-0005`
