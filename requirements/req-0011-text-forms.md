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
- **Lists.** A list box shows its items from the first one in view,
  scrolled so that the selection is in it, the selection white on blue
  while the list has the focus (black on grey otherwise), and a scroll
  bar in its last column (`▲`, `░` with a `█` thumb, `▼`) when the items
  do not fit. Up, Down, Page Up, Page Down, Home and End move the
  selection, and a letter selects the next item that starts with it; the
  model raises `Click`. A combo box is a field with `↓` at its end; the
  same keys choose its item, and F4 or Alt+Down opens its list (up to 8
  items in a box below it, or above it at the bottom of the screen) over
  everything else; Enter, Esc, F4, Alt+Down or leaving it closes it, and
  the Enter or Esc that closes it does not press a button.
- **Scroll bars.** A scroll bar is `◄` and `►` (`▲` and `▼` upright) with a
  track of `░` and a `█` thumb placed by `Value` between `Min` and `Max`.
  Left and Up move it by `SmallChange` toward `Min`, Right and Down toward
  `Max`, Page Up and Page Down by `LargeChange`, Home and End to its ends;
  the model raises `Change`.
- **Timers.** With keys from the console, a `Timer` fires on the clock.
  With keys from the standard input, time passes only when the two bytes
  0, 0 (a key of scan code 0, which no key has) are read: the timers'
  clock moves on to the next one that is due, and it fires, so a program
  with timers runs the same on every machine.
- **Menus.** A form with visible menus has a menu bar on its first row
  inside the border, above its client area: each title as ` text `,
  its access key in white. F10 opens the first menu, Alt and a title's
  access key that menu; its `Click` is raised as it opens. An open menu
  is a single box below its title (an item's menu beside the item) with
  a shadow: `√` before a checked item, `►` after one that opens a menu, a
  line for a `-` item, disabled items in dark grey, the highlighted item
  white on blue. Up and Down move the highlight over the items that can
  be chosen; Right opens the highlighted item's menu or the next title's,
  Left goes back or to the previous title; Enter or an item's access key
  chooses it (its `Click`, after the menus close); Esc closes one menu,
  F10 all of them. A menu item's `Shortcut` (`^N`, `{F5}`, `+{DEL}` as
  the `.frm` writes it) chooses it from anywhere on its form; the form
  model does this for every host.
- **Dialogs.** `MsgBox` and `InputBox` are boxes with a double border in
  white on grey, their title on it and a shadow, centred on the screen
  over the forms: the text in lines (a line break, or a blank where a line
  is full), an input field with its default text and the caret after it
  (`InputBox`), then the buttons: OK; OK and Cancel; Abort, Retry and
  Ignore; Yes, No and Cancel; Yes and No; Retry and Cancel, the default
  one (`vbDefaultButton2`, `3`) with the focus. Tab, Shift+Tab, Left and
  Right move the focus, Enter chooses the focused button (OK from the
  input field), a button's access key (A, R, I, Y, N) chooses it, and
  Esc gives Cancel, or OK when it is the only button. `MsgBox` returns
  the button's value (1 to 7); `InputBox` the text, or `""` for Cancel.
  When the keys end with a dialog open, the screen keeps showing it and
  it gives the answer of Esc (or of its default button).
- **The mouse.** On the console the text host asks for mouse input (and
  turns off Quick Edit while it runs; the console's mode comes back when it
  ends). Mouse events keep their order with the keys (each holds a place in
  the key buffer as the key 0, 255; moves in a row become one). A button
  going down, going up, a double click and a move become `MouseDown`,
  `MouseUp`, `DblClick` and `MouseMove` of the window under the pointer (the
  one drawn last there), with VB's button (1 left, 2 right, 4 middle) and
  shift (1, 2, 4) and the middle of the cell in twips from the window's
  origin (a form's client area); a release goes to the window the button
  went down on, and a move is reported once a cell. A left button released
  over the window it went down on clicks it (`Click`, which also gives it
  the focus), except a double click. A list selects the item clicked (its
  scroll bar's arrows the item before or after); a combo box opens or closes
  its list, and an item of the open list is selected; a scroll bar moves by
  `SmallChange` on its arrows and by `LargeChange` on its track; a text box
  puts its caret at the character clicked. On the menus, a title opens its
  menu and an item is chosen; on a dialog, a button released over it is
  chosen. In a file of keys, a mouse record is the bytes 0, 255, then the
  kind (`D` down, `U` up, `M` move, `2` double click), the buttons, the row
  and the column.
- **Building.** `wfc build` of a project with `WfcTextUI=1` writes a
  console program (not a Windows one), which shows its forms on the text
  screen as `wfc` does.
- **Several forms.** A form that is shown becomes the active one (the
  focus goes to its first tab stop); Ctrl+F6 makes the next visible form
  the active one, and so does a click on it. When the active form is
  unloaded or hidden, the form on top gets the focus back on the control
  that had it last. While a form shown with `Show vbModal` runs, input
  to every other form (keys, clicks, Ctrl+F6, closing) is ignored; the
  form model does this for every host.
- **`DoEvents`** handles the waiting keys, mouse events and timers and
  returns. When the keys come from the standard input and end during
  `DoEvents`, the program goes on and the screen with it; the screen
  stays as it is once the program next waits for input.
- **The end.** The program ends when its last form is unloaded, as in VB6.
  When the keys come from the standard input and it has ended, the forms
  that are still loaded are closed, but the screen keeps showing them: its
  text is what the program printed.

## Known limits

Steps 1 to 5 of M3: a text box has one line (no `MultiLine`, no
`PasswordChar`, no selection); a combo box's text cannot be typed (every
style chooses from its list); a list box selects one item (no
`MultiSelect`, no `Columns`); menus show no shortcut text, and
`PopupMenu` is not drawn; timers do not fire while a dialog is open; the
mouse wheel and dragging (`DragMode`) are not used (WFC ADR-0012).

## Verification

**Method:** Test

**References:** `TC-0012` (the drawing), `TC-0013` (the mouse), `TC-0005` (`320-text-form`,
`321-text-input`, `322-text-lists`, `323-text-combo-open`,
`324-text-menus`, `325-text-msgbox`, `326-text-mouse` and
`327-text-forms` in WFC; in
WFC also `TC-MP0007-package-320-text-form` and `...-326-text-mouse`,
built with `wfc build`)
