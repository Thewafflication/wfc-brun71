' The viewport scrolls; the bottom line is outside it until VIEW PRINT.
CLS
LOCATE 1, 1: PRINT "top"
LOCATE 25, 1: PRINT "bottom line";
LOCATE 20, 1
FOR i = 1 TO 6: PRINT "scroll"; i: NEXT
VIEW PRINT
LOCATE 25, 1: PRINT "whole";
PRINT
PRINT "last";
