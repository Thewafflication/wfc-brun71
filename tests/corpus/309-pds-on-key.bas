' ON KEY(n) GOSUB, KEY(n) ON, STOP and OFF, and a user-defined key, with
' the keys of 309-pds-on-key.keys. Before each statement (a label line too)
' one key is read when none is waiting, so the handlers run as soon as their
' keys come: Q (trapped inside the F1 handler) before F1 ends. Native code
' does not compile GOSUB yet, so wfc runs this program in the interpreter in
' both modes.
CLS
ON KEY(1) GOSUB F1Pressed
ON KEY(11) GOSUB UpPressed
KEY 15, CHR$(0) + CHR$(16)
ON KEY(15) GOSUB QPressed
KEY(1) ON
KEY(11) STOP
KEY(15) ON
DO
    k$ = INKEY$
    IF k$ = "x" THEN EXIT DO
    IF k$ <> "" THEN PRINT "key "; k$
LOOP
PRINT "up waits"
KEY(11) ON
PRINT "after on"
KEY(1) OFF
PRINT "then "; INKEY$
k$ = INKEY$
PRINT "F1 off:"; LEN(k$); ASC(MID$(k$, 2, 1))
ON ERROR GOTO Trap
KEY(26) ON
ON KEY(0) GOSUB F1Pressed
PRINT "end"
END
F1Pressed:
    PRINT "F1 handler"
    RETURN
UpPressed:
    PRINT "Up handler"
    RETURN
QPressed:
    PRINT "Q handler"
    RETURN
Trap:
    PRINT "error"; ERR
    RESUME NEXT
