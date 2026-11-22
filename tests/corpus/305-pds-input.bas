' INPUT, LINE INPUT and INPUT$ read the keys of 305-pds-input.keys: the
' prompt and "? ", the typed line on the screen, Backspace, "Redo from
' start" for a field that is not a number or out of range, INPUT; and
' quoted fields.
CLS
INPUT "Name"; nm$
PRINT "Hello, "; nm$
INPUT "Two numbers", a, b
PRINT a + b
INPUT x%
PRINT x% * 2
LINE INPUT "Line: "; l$
PRINT "["; l$; "]"
INPUT; "Stay"; s$
PRINT "<"; s$; ">"
INPUT "Quoted"; q$, n
PRINT q$; n
k$ = INPUT$(3)
PRINT "Keys: "; k$
