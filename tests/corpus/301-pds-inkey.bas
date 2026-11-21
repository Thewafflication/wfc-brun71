' INKEY$ reads the keys of 301-pds-inkey.keys from the standard input:
' a letter, an extended key (CHR$(0) + scan code), Enter, then nothing.
CLS
FOR n = 1 TO 4
  k$ = INKEY$
  IF LEN(k$) = 2 THEN
    PRINT "extended"; ASC(MID$(k$, 2))
  ELSEIF LEN(k$) = 1 THEN
    PRINT "key"; ASC(k$)
  ELSE
    PRINT "none"
  END IF
NEXT
