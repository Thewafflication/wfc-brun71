' PDS 7.1 text screen: plain output before the screen, then LOCATE,
' PRINT zones, TAB, SPC, COLOR, CSRLIN, POS and VIEW PRINT.
PRINT "before the screen"
CLS
LOCATE 2, 5: PRINT "row 2 col 5"
PRINT 1; -2; 3.5
PRINT "a", "b", "c"
PRINT TAB(10); "tab10"; SPC(3); "spc"
COLOR 14, 1: PRINT "yellow";
PRINT " row"; CSRLIN; "pos"; POS(0)
PRINT STRING$(75, "x"); "fits."
PRINT STRING$(75, "y"); "wraps"; "!"
VIEW PRINT 12 TO 14
FOR i = 1 TO 5: PRINT "line"; i: NEXT
VIEW PRINT
LOCATE 16, 1: PRINT "row 16"
