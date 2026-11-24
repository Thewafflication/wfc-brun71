VERSION 5.00
Begin VB.Form Form1
   Caption         =   "Main"
   ClientHeight    =   1920
   ClientWidth     =   4320
   Begin VB.CommandButton cmdAsk
      Caption         =   "&Ask"
      Height          =   240
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1200
   End
   Begin VB.CommandButton cmdTools
      Caption         =   "&Tools"
      Height          =   240
      Left            =   1560
      TabIndex        =   1
      Top             =   240
      Width           =   1200
   End
   Begin VB.CommandButton cmdCount
      Caption         =   "&Count"
      Height          =   240
      Left            =   2880
      TabIndex        =   2
      Top             =   240
      Width           =   1200
   End
   Begin VB.Label Label1
      Caption         =   "No name yet."
      Height          =   240
      Left            =   240
      Top             =   960
      Width           =   3840
   End
   Begin VB.Label Label2
      Caption         =   ""
      Height          =   240
      Left            =   240
      Top             =   1440
      Width           =   3840
   End
End
Attribute VB_Name = "Form1"
Option Explicit
Public StopRequested As Boolean

Private Sub Form_Load()
    Left = 240
    Top = 480
End Sub

Private Sub cmdAsk_Click()
    Form2.Show vbModal
    Label1.Caption = "Hello, " & Form2Answer & "."
End Sub

Private Sub cmdTools_Click()
    Form3.Show
End Sub

Private Sub cmdCount_Click()
    Dim i As Long
    StopRequested = False
    Label2.Caption = "Counting..."
    For i = 1 To 1000000
        DoEvents
        If StopRequested Then Exit For
    Next
    Label2.Caption = "Stopped after " & i & " pass(es)."
End Sub
