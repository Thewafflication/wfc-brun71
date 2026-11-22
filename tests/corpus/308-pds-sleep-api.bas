' A VB6 module that declares the Windows Sleep keeps calling it: no text
' screen starts and the plain output stays.
Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Sleep 10
Print "slept"
