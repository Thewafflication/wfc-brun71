# REQ-0003 — Code page 437 and the Windows console host

**Content type:** Project requirement

**Status:** Proposed

## Statement

- **Code page 437.** Every code 1-255 shows as its code page 437 Unicode
  character, codes 1-31 and 127 as the screen glyphs (smiley, arrows, house);
  code 0 shows blank. The mapping is one-to-one for codes 1-255, so a
  character typed or pasted maps back to its code; control characters map to
  themselves; a character outside the code page has no code.
- **Console host (Windows).** The host opens a console screen buffer of its
  own, active (in place of the current one until the host is closed) or not.
  Showing a screen writes every cell with its character (code page 437 to
  Unicode) and attribute; the blink bit is dropped. The cursor goes to the
  screen's cursor with its visibility and a size of the scan lines from start
  to stop as a share of 8. The buffer takes the screen's size. Keys typed in
  the console are translated (REQ-0002) into the key buffer.
- **No console.** Opening the host in a process without a console fails
  cleanly (no host), so the caller can fall back.

## Verification

**Method:** Test

**References:** `TC-0003`, `TC-0004`
