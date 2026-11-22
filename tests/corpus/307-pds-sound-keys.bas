' SOUND, PLAY, BEEP, SLEEP and the soft keys. The output is redirected, so
' nothing is heard and no time passes; SLEEP ends on the key of
' 307-pds-sound-keys.keys, which INKEY$ then reads.
CLS
KEY 1, "LIST"
KEY 2, "RUN" + CHR$(13)
KEY 10, "SCREEN"
KEY ON
CLS 0
KEY LIST
SOUND 440, 2
PLAY "T200 O3 L8 CDEFGAB > C"
PLAY "MS P4 N46"
BEEP
SLEEP
PRINT "slept until "; INKEY$
ON ERROR GOTO Trap
SOUND 20, 1
PLAY "O9"
PLAY "X"
KEY 13, "x"
PLAY 5
PRINT "done"
END
Trap:
PRINT "error"; ERR
RESUME NEXT
