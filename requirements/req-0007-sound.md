# REQ-0007 — `SOUND`, `PLAY`, `BEEP` and `SLEEP`

**Content type:** Project requirement

**Status:** Proposed (no BRUN71 reference probe; the assumed rules are listed
in `COMPATIBILITY.md`)

## Statement

- **SOUND frequency, duration** plays `frequency` Hz (37 to 32767) for
  `duration` clock ticks (0 to 65535, 18.2 a second); an argument out of
  range raises error 5.
- **PLAY music$** plays the music macro language: notes `A`-`G` with `#`,
  `+` or `-`, a length 1-64 and dots (each 1.5 times longer); `On` (0-6,
  `O3` starts at middle C, a program starts in `O4`), `<` and `>` (staying
  within 0-6); `Ln` (1-64, start 4); `Tn` (32-255 quarter notes a minute,
  start 120); `Pn` a pause; `Nn` note n (1-84, 0 a rest; N46 is A, 440 Hz);
  `MN` (7/8 of the length sounds), `ML` (all), `MS` (3/4); `MF` and `MB`.
  Blanks are ignored. Octave, length, tempo and style last from one `PLAY`
  to the next. Anything else, a missing or out-of-range number, or `X`,
  raises error 5.
- **BEEP** plays a short tone (800 Hz, a quarter of a second).
- **Hearing.** The tones are played on the console's speaker when the screen
  is shown on a console; otherwise nothing is played and no time passes.
- **SLEEP [seconds]** waits the whole seconds given, or with 0 or none until
  a key; a key ends it at once (one already waiting too) and stays for
  `INKEY$`. With keys from the standard input, a key there ends it at once;
  once that input has ended the time passes, and `SLEEP` with no time
  returns.
- **In WFC** the words are statements in a standard module unless the
  program has a procedure of that name (a `Declare Sub Sleep` keeps calling
  Windows) or they are followed by `=`, `.`, `#` or `(`.

## Verification

**Method:** Test

**References:** `TC-0008` (the tones), `TC-0005` (`307-pds-sound-keys`,
`308-pds-sleep-api` in WFC)
