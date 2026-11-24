VERSION 5.00
Begin VB.Form Form2
   Caption         =   "Name"
   ClientHeight    =   960
   ClientWidth     =   3360
   Begin VB.TextBox txtName
      Height          =   240
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1800
   End
   Begin VB.CommandButton cmdOK
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   240
      Left            =   2160
      TabIndex        =   1
      Top             =   240
      Width           =   960
   End
End
Attribute VB_Name = "Form2"
Option Explicit

Private Sub cmdOK_Click()
    Form2Answer = txtName.Text
    Unload Me
End Sub
