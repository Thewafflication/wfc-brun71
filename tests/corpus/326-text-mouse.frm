VERSION 5.00
Begin VB.Form Form1
   Caption         =   "Mouse"
   ClientHeight    =   2640
   ClientWidth     =   5280
   Begin VB.CommandButton Command1
      Caption         =   "&Press"
      Height          =   240
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1440
   End
   Begin VB.CheckBox Check1
      Caption         =   "&Sound"
      Height          =   240
      Left            =   240
      TabIndex        =   1
      Top             =   720
      Width           =   1440
   End
   Begin VB.TextBox Text1
      Height          =   240
      Left            =   240
      TabIndex        =   2
      Text            =   "abcdef"
      Top             =   1200
      Width           =   1440
   End
   Begin VB.ListBox List1
      Height          =   720
      Left            =   2400
      TabIndex        =   3
      Top             =   240
      Width           =   1920
   End
   Begin VB.Label Label1
      Caption         =   ""
      Height          =   240
      Left            =   240
      Top             =   1680
      Width           =   4800
   End
   Begin VB.Label Label2
      Caption         =   ""
      Height          =   240
      Left            =   240
      Top             =   1920
      Width           =   4800
   End
   Begin VB.Label Label3
      Caption         =   ""
      Height          =   240
      Left            =   240
      Top             =   2160
      Width           =   4800
   End
End
Attribute VB_Name = "Form1"
Option Explicit

Private Sub Form_Load()
    List1.AddItem "one"
    List1.AddItem "two"
    List1.AddItem "three"
    List1.AddItem "four"
End Sub

Private Sub Command1_Click()
    Label1.Caption = Label1.Caption & "Press "
End Sub

Private Sub Check1_Click()
    Label1.Caption = Label1.Caption & "Sound=" & Check1.Value & " "
End Sub

Private Sub List1_Click()
    Label2.Caption = "List: " & List1.Text
End Sub

Private Sub List1_DblClick()
    Label2.Caption = Label2.Caption & " (double)"
End Sub

Private Sub Form_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Label3.Caption = "Form down " & Button & " at " & X & "," & Y
End Sub
