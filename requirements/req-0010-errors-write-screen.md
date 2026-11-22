# REQ-0010 — Run-time error messages, `WRITE` and `SCREEN()`

**Content type:** Project requirement

**Status:** Proposed (no BRUN71 reference probe; the assumed rules are listed
in `COMPATIBILITY.md`)

## Statement

- **Messages.** The runtime knows BRUN71's message for each run-time error
  of the PDS 7.1 list (2 Syntax error ... 76 Path not found, ISAM errors
  excluded), for example 3 "RETURN without GOSUB", 5 "Illegal function
  call", 16 "String formula too complex"; another number is "Unprintable
  error".
- **An error no handler takes** ends a program that has started the screen
  with the message on a line of its own on the screen (after the text on
  the cursor's line), and the same message in WFC's report; WFC exits with
  status 1.
- **WRITE [items]** prints the items as `WRITE #` writes them (strings in
  quotes, numbers without blanks and without the 0 before a point, commas
  between them), then a new line: on the screen once it has started, in the
  plain output before. `WRITE #n` keeps writing files.
- **SCREEN(row, column [, colorflag])** returns the character code of a
  screen cell, or with a colour flag that is not 0 its attribute; a cell
  outside the screen raises error 5. In WFC the function needs its
  parentheses; `Screen` without them is the VB6 `Screen` object.

## Verification

**Method:** Test

**References:** `TC-0011` (the messages), `TC-0001` (`SCREEN()` on the
screen model), `TC-0005` (`310-pds-write-screen`, `311-pds-error-exit` in
WFC)
