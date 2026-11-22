' WRITE to the plain output and to the screen, and SCREEN(row, col [, flag]):
' the character and the attribute of a cell; a cell outside is error 5.
WRITE "plain", 1, -2.5, 0.25
WRITE
CLS
COLOR 14, 1
PRINT "AB"
COLOR 7, 0
WRITE "a b", 3; 4
PRINT SCREEN(1, 1); SCREEN(1, 2, 1); SCREEN(2, 1)
ON ERROR GOTO Trap
x = SCREEN(26, 1)
PRINT "end"
END
Trap:
PRINT "error"; ERR
RESUME NEXT
