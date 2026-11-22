# REQ-0006 — `PRINT USING`

**Content type:** Project requirement

**Status:** Proposed (no BRUN71 reference probe; the assumed rules are listed
in `COMPATIBILITY.md`)

## Statement

- **Syntax.** `PRINT [#n,] USING format$; expression [{; | ,} expression ...]
  [{; | ,}]` prints the values formatted by the fields of `format$`; a
  separator at the end keeps the line open. On the screen, in a file and in
  the plain output alike.
- **String fields.** `!` the first character; `\  \` two characters plus one
  per blank between the backslashes, padded with blanks; `&` the whole
  string.
- **Numeric fields.** `#` a digit position; `.` the decimal point; `,` before
  the point a position that also groups the whole part by thousands; `+`
  first or last prints the sign there; `-` last prints a minus or a blank;
  `**` fills the left with asterisks; `$$` puts a dollar sign before the
  number; `**$` does both; `^^^^` and `^^^^^` print an exponent of two or
  three digits. Without a sign field a negative number takes a position for
  its minus (in an exponent field the position is kept for it). The number
  is rounded to the decimals, halves away from zero; a lone 0 before the
  point goes when there is no room for it.
- **Overflow.** A number wider than its field is printed whole after a `%`.
- **Literals.** `_` prints the next character as it is; any other character
  of the format is printed as it is.
- **Lists.** The format starts again from its beginning while values are
  left; after the last value, the format's text up to the next field is
  printed.
- **Errors.** A format without a field, given a value, raises error 5; a
  string in a numeric field or a number in a string field raises error 13.

## Verification

**Method:** Test

**References:** `TC-0007` (the formatter), `TC-0005` (`306-pds-using` in WFC)
