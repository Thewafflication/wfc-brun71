# REQ-0011 — VB6 forms in text mode

**Content type:** Project requirement

**Status:** Proposed (M3, WFC ADR-0012; the look is this project's own, not
taken from a reference program)

## Statement

- **Drawing.** The library draws a text user interface on the screen: a
  region filled with an attribute; its border with single lines
  (`┌─┐│└┘`) or double lines (`╔═╗║╚╝`); a title, `" text "`, centred on
  the top border and cut to fit; the shadow of a window (the two columns to
  its right from its second row, and the row below it from its third
  column, keep their characters in dark grey on black); text cut to a width.
  Cells off the screen are not drawn.
- **Captions.** `&` marks the access key of a caption and is not shown;
  `&&` is one `&`. A command button's face is `< text >`, centred in the
  button's width and cut to it.
- **VB6 forms in WFC.** A project with `WfcTextUI=1`, or run with
  `wfc --text-ui`, shows its forms on the text screen instead of in
  windows. A character cell is 120 twips wide and 240 high. A form is a box
  of black on grey: its client area plus a border, with its caption as the
  title and a shadow; the active form has a double border in white on
  grey, another form a single one. Labels show their caption; a command
  button its face, on the middle row of its cells, white on blue while it
  has the focus. The focused control has the cursor.
- **Keys.** Keys go to the control with the focus as `KeyDown`, `KeyPress`
  and `KeyUp`; Tab and Shift+Tab move the focus, Enter presses the focused
  or the `Default` button, Esc the `Cancel` button, Space the focused
  button, Alt and an access key the control it marks (the form model of
  WFC does this for every host). Alt+F4 closes the active form.
- **Typing.** A text box shows one line of its text, scrolled so that the
  caret is in it, and keeps its caret: a character the program's
  `KeyPress` leaves (`KeyAscii` not 0, perhaps changed) goes in at the
  caret; Backspace deletes before it, Delete after it; Left, Right, Home
  and End move it. The model takes the new text and raises `Change`. A
  `Locked` text box takes no typing, and one at its `MaxLength` no more
  characters. A text the program sets puts the caret at its start.
- **Option buttons and frames.** The arrow keys on an option button
  select the next (Right, Down) or previous (Left, Up) option button of
  its container, ordered from top to bottom and left to right, as a
  click does. A frame is a single box with its caption on the top
  border; its controls are placed from its top-left cell.
- **The end.** The program ends when its last form is unloaded, as in VB6.
  When the keys come from the standard input and it has ended, the forms
  that are still loaded are closed, but the screen keeps showing them: its
  text is what the program printed.

## Known limits

Steps 1 and 2 of M3: a text box has one line (no `MultiLine`, no
`PasswordChar`, no selection); lists, combo boxes, scroll bars, menus,
`MsgBox`, `InputBox` and the mouse come in later steps (WFC ADR-0012).

## Verification

**Method:** Test

**References:** `TC-0012` (the drawing), `TC-0005` (`320-text-form` and
`321-text-input` in WFC)
