# REQ-0009 — Key trapping: `ON KEY(n) GOSUB`, `KEY(n) ON/OFF/STOP`

**Content type:** Project requirement

**Status:** Proposed (no BRUN71 reference probe; the assumed rules are listed
in `COMPATIBILITY.md`)

## Statement

- **Keys.** 1-10 F1-F10, 11 Up, 12 Left, 13 Right, 14 Down, 15-25 defined
  by the program, 30 F11, 31 F12; any other number raises error 5.
- **User-defined keys.** `KEY n, CHR$(flags) + CHR$(scancode)` with n 15-25
  defines a key by its scan code (US layout): flags 0 the key alone, 1-3
  with Shift, 4 with Ctrl, 8 with Alt, 128 an extended key; 32 (Num Lock)
  and 64 (Caps Lock) are ignored. A text that is not two characters raises
  error 5.
- **ON KEY(n) GOSUB label** sets the handler of key n; `GOSUB 0` removes it.
- **KEY(n) ON** traps the key: from then on the key is taken from the
  keyboard before `INKEY$`, `INPUT` or `SLEEP` see it, and its handler runs
  before the next statement. **KEY(n) STOP** still takes the key, but the
  event waits until `KEY(n) ON`. **KEY(n) OFF** leaves the key to the
  program and forgets a waiting event. A key without a handler is never
  taken.
- **Handler.** While the handler runs the key is stopped; its `RETURN`
  turns it on again, unless the handler turned it off. A trapped key ends a
  `SLEEP`.
- **When.** Events are checked before each statement of the module level
  (outside `SUB` and `FUNCTION` procedures); before each check the keys
  typed meanwhile are read (from the standard input, one key when none is
  waiting).

## Verification

**Method:** Test

**References:** `TC-0010` (the traps), `TC-0005` (`309-pds-on-key` in WFC)
