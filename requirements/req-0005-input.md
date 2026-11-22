# REQ-0005 — `INPUT`, `LINE INPUT` and `INPUT$`

**Content type:** Project requirement

**Status:** Proposed (no BRUN71 reference probe; the assumed rules are listed
in `COMPATIBILITY.md`)

## Statement

- **Syntax.** `INPUT [;] ["prompt" {; | ,}] variable [, variable ...]` and
  `LINE INPUT [;] ["prompt";] variable$` read from the keyboard; `INPUT #n`
  and `LINE INPUT #n` keep reading files.
- **Prompt.** The prompt is printed at the cursor; `INPUT` adds "? " after a
  prompt followed by a semicolon and when there is no prompt. `LINE INPUT`
  adds nothing.
- **Line editor.** The characters typed (codes 32 and up, at most 255) show
  at the cursor, wrapping and scrolling as `PRINT` does; Backspace erases the
  last one, also across a wrapped line; Esc erases the whole line; Enter ends
  it. Other keys do nothing.
- **After Enter.** The cursor goes to the start of the next line, or stays
  after the line with `INPUT;` / `LINE INPUT;`.
- **Fields.** `INPUT` splits the line at its commas. A quoted field keeps what
  is between the quotes; an unquoted one loses the blanks around it. A
  numeric field takes a sign, decimals, an `E` or `D` exponent, `&H` or `&O`
  digits and one type suffix; an empty one is 0. An empty line gives every
  variable 0 or "".
- **Redo from start.** When the number of fields differs from the number of
  variables, a numeric field is not a number, is quoted or is out of the
  variable's range, or text follows a closing quote, the statement prints
  "Redo from start" on a line of its own and asks again (prompt included); no
  variable changes.
- **`LINE INPUT`** stores the whole line, blanks and commas included, in one
  String variable.
- **`INPUT$(n)`** returns the next `n` keys (1 to 32767) without showing them;
  an extended key counts as two characters, `CHR$(0)` and its code.
- **Keys.** The keys come from the keyboard of REQ-0002 (in WFC, the console
  or the standard input, REQ-0004); when the standard input ends before the
  line does, error 62 (Input past end of file).

## Verification

**Method:** Test

**References:** `TC-0006` (line editor, fields, numbers), `TC-0005`
(`305-pds-input` in WFC)
