VERSION 5.00
Begin VB.Form Form3
   Caption         =   "Tools"
   ClientHeight    =   960
   ClientWidth     =   2640
   Begin VB.CommandButton cmdStop
      Caption         =   "&Stop"
      Height          =   240
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1200
   End
End
Attribute VB_Name = "Form3"
Option Explicit

Private Sub Form_Load()
    Left = 5760
    Top = 480
End Sub

Private Sub cmdStop_Click()
    Form1.StopRequested = True
End Sub
