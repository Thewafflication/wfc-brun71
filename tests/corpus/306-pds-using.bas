' PRINT USING to the plain output, to a file and on the screen: digits,
' thousands, signs, $$ and ** fills, exponents, string fields, a format
' used again for more values, and errors 13 and 5.
PRINT USING "Total: ###.## units"; 42.5
PRINT USING "##"; 1; 2; 3
PRINT USING "& has $$#,###.##"; "Ann"; 1234.5; "Bob"; -7
PRINT USING "**#.#"; 1.25;
PRINT " after"
PRINT USING "\  \|!|"; "abcdef"; "xyz"
PRINT USING "##.##^^^^"; 234.56
PRINT USING "###"; 12345
' A name of its own: the interpreted and native runs may overlap.
RANDOMIZE TIMER
f$ = "306-pds-using-" + LTRIM$(STR$(INT(RND * 1000000))) + ".tmp"
OPEN f$ FOR OUTPUT AS #1
PRINT #1, USING "##.#"; 2.25
CLOSE #1
OPEN f$ FOR INPUT AS #1
LINE INPUT #1, t$
CLOSE #1
KILL f$
PRINT "["; t$; "]"
ON ERROR GOTO Trap
PRINT USING "###"; "text"
PRINT USING "none"; 1
CLS
LOCATE 2, 5
PRINT USING "+##.#"; 3.14; -2
END
Trap:
PRINT "Error" + STR$(ERR)
RESUME NEXT
