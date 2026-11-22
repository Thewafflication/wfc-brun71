' An error no handler takes ends a PDS program with BRUN71's message on the
' screen (and in the report on the standard error); wfc exits with 1
' (311-pds-error-exit.exitcode).
CLS
PRINT "before the error"
x = 1 / 0
PRINT "not printed"
