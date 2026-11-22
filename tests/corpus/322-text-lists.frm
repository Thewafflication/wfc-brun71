VERSION 5.00
Begin VB.Form Form1
   Caption         =   "Lists"
   ClientHeight    =   2880
   ClientWidth     =   5280
   Begin VB.ListBox List1
      Height          =   960
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1800
   End
   Begin VB.ComboBox Combo1
      Height          =   240
      Left            =   2640
      Style           =   2  'Dropdown List
      TabIndex        =   1
      Top             =   240
      Width           =   1800
   End
   Begin VB.HScrollBar HScroll1
      Height          =   240
      LargeChange     =   3
      Left            =   240
      Max             =   10
      TabIndex        =   2
      Top             =   1440
      Value           =   5
      Width           =   2400
   End
   Begin VB.Timer Timer1
      Interval        =   1000
      Left            =   4560
      Top             =   1440
   End
   Begin VB.Label Label1
      Caption         =   "List:"
      Height          =   240
      Left            =   240
      Top             =   1920
      Width           =   4800
   End
   Begin VB.Label Label2
      Caption         =   "Scroll: 5"
      Height          =   240
      Left            =   240
      Top             =   2160
      Width           =   4800
   End
   Begin VB.Label Label3
      Caption         =   "Ticks: 0"
      Height          =   240
      Left            =   240
      Top             =   2400
      Width           =   4800
   End
End
Attribute VB_Name = "Form1"
Option Explicit
Dim ticks As Long

Private Sub Form_Load()
    Dim fruit As Variant
    For Each fruit In Array("Apple", "Banana", "Cherry", "Date", "Elder", "Fig")
        List1.AddItem fruit
    Next
    Combo1.AddItem "Red"
    Combo1.AddItem "Green"
    Combo1.AddItem "Blue"
    Combo1.ListIndex = 0
End Sub

Private Sub List1_Click()
    Label1.Caption = "List: " & List1.Text & " (" & List1.ListIndex & ")"
End Sub

Private Sub Combo1_Click()
    Label1.Caption = "Combo: " & Combo1.Text
End Sub

Private Sub HScroll1_Change()
    Label2.Caption = "Scroll: " & HScroll1.Value
End Sub

Private Sub Timer1_Timer()
    ticks = ticks + 1
    Label3.Caption = "Ticks: " & ticks
    If ticks = 2 Then Timer1.Enabled = False
End Sub
