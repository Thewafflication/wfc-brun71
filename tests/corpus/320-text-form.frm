VERSION 5.00
Begin VB.Form Form1
   Caption         =   "Greeting"
   ClientHeight    =   2400
   ClientWidth     =   4800
   StartUpPosition =   2  'CenterScreen
   Begin VB.Label Label1
      Caption         =   "Press a button."
      Height          =   240
      Left            =   240
      Top             =   240
      Width           =   4320
   End
   Begin VB.CheckBox Check1
      Caption         =   "&Loud"
      Height          =   240
      Left            =   240
      TabIndex        =   0
      Top             =   720
      Width           =   2400
   End
   Begin VB.CommandButton Command1
      Caption         =   "&Hello"
      Default         =   -1  'True
      Height          =   240
      Left            =   240
      TabIndex        =   1
      Top             =   1680
      Width           =   1440
   End
   Begin VB.CommandButton Command2
      Caption         =   "&Quit"
      Cancel          =   -1  'True
      Height          =   240
      Left            =   2880
      TabIndex        =   2
      Top             =   1680
      Width           =   1440
   End
End
Attribute VB_Name = "Form1"
Option Explicit
Dim presses As Long

Private Sub Command1_Click()
    presses = presses + 1
    If Check1.Value = 1 Then
        Label1.Caption = "HELLO! (" & presses & ")"
    Else
        Label1.Caption = "Hello. (" & presses & ")"
    End If
End Sub

Private Sub Command2_Click()
    Unload Me
End Sub

Private Sub Check1_Click()
    Caption = "Greeting" & IIf(Check1.Value = 1, " - loud", "")
End Sub
