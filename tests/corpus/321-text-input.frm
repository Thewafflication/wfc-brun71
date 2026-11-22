VERSION 5.00
Begin VB.Form Form1
   Caption         =   "Input"
   ClientHeight    =   2640
   ClientWidth     =   5280
   Begin VB.TextBox Text1
      Height          =   240
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   960
   End
   Begin VB.TextBox Text2
      Height          =   240
      Left            =   240
      MaxLength       =   5
      TabIndex        =   1
      Top             =   720
      Width           =   960
   End
   Begin VB.Frame Frame1
      Caption         =   "Colour"
      Height          =   1200
      Left            =   2400
      TabIndex        =   6
      Top             =   0
      Width           =   2400
      Begin VB.OptionButton Option1
         Caption         =   "Red"
         Height          =   240
         Index           =   0
         Left            =   240
         TabIndex        =   2
         Top             =   240
         Value           =   -1  'True
         Width           =   1800
      End
      Begin VB.OptionButton Option1
         Caption         =   "Green"
         Height          =   240
         Index           =   1
         Left            =   240
         TabIndex        =   3
         Top             =   480
         Width           =   1800
      End
      Begin VB.OptionButton Option1
         Caption         =   "Blue"
         Height          =   240
         Index           =   2
         Left            =   240
         TabIndex        =   4
         Top             =   720
         Width           =   1800
      End
   End
   Begin VB.TextBox Text3
      Height          =   240
      Left            =   240
      Locked          =   -1  'True
      TabIndex        =   5
      Text            =   "fixed"
      Top             =   1200
      Width           =   960
   End
   Begin VB.Label Label1
      Caption         =   "Text1:"
      Height          =   240
      Left            =   240
      Top             =   1680
      Width           =   4800
   End
   Begin VB.Label Label2
      Caption         =   "Colour: Red"
      Height          =   240
      Left            =   240
      Top             =   1920
      Width           =   4800
   End
   Begin VB.Label Label3
      Caption         =   "Text2:"
      Height          =   240
      Left            =   240
      Top             =   2160
      Width           =   4800
   End
End
Attribute VB_Name = "Form1"
Option Explicit

Private Sub Text1_Change()
    Label1.Caption = "Text1: [" & Text1.Text & "]"
End Sub

Private Sub Text2_KeyPress(KeyAscii As Integer)
    KeyAscii = Asc(UCase$(Chr$(KeyAscii)))
End Sub

Private Sub Text2_Change()
    Label3.Caption = "Text2: [" & Text2.Text & "] Text3: [" & Text3.Text & "]"
End Sub

Private Sub Option1_Click(Index As Integer)
    Label2.Caption = "Colour: " & Option1(Index).Caption
End Sub
