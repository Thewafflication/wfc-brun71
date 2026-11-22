' A VB6 module that only uses the words of the PDS statements as names:
' it keeps the plain output (no text screen starts).
Dim color(3) As Integer
Dim pos As Integer
color(1) = 5
pos = 2
Print color(1); pos
width = 40
Print width
' A name of its own: the interpreted and native runs may overlap.
Randomize Timer
Dim f As String
f = "303-pds-names-" & CStr(Int(Rnd * 1000000)) & ".tmp"
Open f For Output As #1
Width #1, 20
Print #1, "x"
Close #1
Kill f
Print "plain output"
