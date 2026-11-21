# REQ-0002 — Keyboard and `INKEY$`

**Content type:** Project requirement

**Status:** Proposed (no BRUN71 reference probe; codes follow the PC BIOS
keyboard codes that PDS 7.1 reports)

## Statement

- **INKEY$ strings.** A character key gives a one-character string (its code
  page 437 code; Enter 13, Esc 27, Backspace 8, Tab 9, Ctrl+A to Ctrl+Z 1-26).
  An extended key gives two characters, `CHR$(0)` and its BIOS code. With no
  key waiting, `INKEY$` is the empty string.
- **Extended codes.** Home 71, Up 72, PgUp 73, Left 75, Right 77, End 79,
  Down 80, PgDn 81, Ins 82, Del 83; with Ctrl 119, 141, 132, 115, 116, 117,
  145, 118, 146, 147; with Alt 151, 152, 153, 155, 157, 159, 154, 161, 162,
  163. F1-F10 59-68, with Shift 84-93, Ctrl 94-103, Alt 104-113; F11 and F12
  133 and 134, with Shift 135-136, Ctrl 137-138, Alt 139-140. Shift+Tab 15.
  Alt+letter the scan code of the letter (Alt+Q 16 ... Alt+M 50), Alt+1 to
  Alt+9 120-128, Alt+0 129, Alt+- 130, Alt+= 131. Alt takes precedence over
  Ctrl, Ctrl over Shift.
- **Windows keys.** Key presses are translated from the Windows virtual-key
  code, the character typed and the Shift, Ctrl and Alt state; Ctrl+Alt with
  a character (AltGr) types the character. Shift, Ctrl, Alt, Caps Lock, Num
  Lock, Scroll Lock alone and characters outside code page 437 give no key.
- **Buffer.** Keys wait in a first-in first-out buffer of 15 keys; a key added
  to a full buffer is refused (lost) and the host may beep.

## Verification

**Method:** Test

**References:** `TC-0002`
