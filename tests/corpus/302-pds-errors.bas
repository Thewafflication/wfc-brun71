' Arguments out of range raise error 5 (Illegal function call).
ON ERROR GOTO Trap
CLS
LOCATE 26, 1
LOCATE 1, 81
LOCATE , , 2
COLOR 32
COLOR , 8
WIDTH 81
VIEW PRINT 10 TO 5
CLS 3
LOCATE 20, 1: PRINT "fine"; CSRLIN
END
Trap:
PRINT "error"; ERR
RESUME NEXT
