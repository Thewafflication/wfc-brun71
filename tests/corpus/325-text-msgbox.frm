VERSION 5.00
Begin VB.Form Form1
   Caption         =   "Files"
   ClientHeight    =   1440
   ClientWidth     =   4800
   Begin VB.CommandButton Command1
      Caption         =   "&Delete"
      Default         =   -1  'True
      Height          =   240
      Left            =   1680
      Top             =   720
      Width           =   1440
   End
End
Attribute VB_Name = "Form1"
Option Explicit

Private Sub Command1_Click()
    If MsgBox("Delete the file?" & vbCrLf & "It cannot be undone.", _
              vbOKCancel + vbExclamation + vbDefaultButton2, "Confirm") = vbOK Then
        Command1.Caption = "Deleted"
    End If
End Sub
