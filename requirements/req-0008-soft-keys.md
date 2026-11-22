# REQ-0008 — Soft keys: `KEY n`, `KEY ON`, `KEY OFF`, `KEY LIST`

**Content type:** Project requirement

**Status:** Proposed (no BRUN71 reference probe; the assumed rules are listed
in `COMPATIBILITY.md`)

## Statement

- **KEY n, text$** gives function key n (1 to 12; error 5 otherwise) a text,
  cut to 15 characters. A program starts with every text empty.
- **KEY ON** shows the texts on the bottom line of the screen: 8 columns a
  key, its number in the normal colours, then the first 6 characters of its
  text (5 for key 10; control characters as blanks) in reverse video (black
  on white); as many of keys 1 to 10 as the width holds (10 on 80 columns, 5
  on 40). The cursor and the text viewport do not change. `KEY n` redraws a
  shown line, and so do `CLS` and `WIDTH` after they clear it.
- **KEY OFF** clears the bottom line.
- **KEY LIST** prints every text, one a line, as `Fn text`.

## Known limits

`KEY(n) ON/OFF/STOP`, `ON KEY(n) GOSUB` and the user-defined keys 15-25 are
planned (ROADMAP M2). Pressing a function key does not type its text.

## Verification

**Method:** Test

**References:** `TC-0009` (the line), `TC-0005` (`307-pds-sound-keys` in WFC)
