VERSION 5.00
Begin VB.Form Form1
   Caption         =   "Editor"
   ClientHeight    =   2400
   ClientWidth     =   5760
   Begin VB.Label Label1
      Caption         =   ""
      Height          =   240
      Left            =   240
      Top             =   1440
      Width           =   5280
   End
   Begin VB.Label Label2
      Caption         =   ""
      Height          =   240
      Left            =   240
      Top             =   1680
      Width           =   5280
   End
   Begin VB.Label Label3
      Caption         =   ""
      Height          =   240
      Left            =   240
      Top             =   1920
      Width           =   5280
   End
   Begin VB.Menu mnuFile
      Caption         =   "&File"
      Begin VB.Menu mnuNew
         Caption         =   "&New"
         Shortcut        =   ^N
      End
      Begin VB.Menu mnuOpen
         Caption         =   "&Open..."
      End
      Begin VB.Menu mnuSep
         Caption         =   "-"
      End
      Begin VB.Menu mnuExit
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu mnuEdit
      Caption         =   "&Edit"
      Begin VB.Menu mnuBold
         Caption         =   "&Bold"
      End
      Begin VB.Menu mnuColour
         Caption         =   "&Colour"
         Begin VB.Menu mnuRed
            Caption         =   "&Red"
         End
         Begin VB.Menu mnuGreen
            Caption         =   "&Green"
            Enabled         =   0   'False
         End
         Begin VB.Menu mnuBlue
            Caption         =   "B&lue"
         End
      End
   End
End
Attribute VB_Name = "Form1"
Option Explicit
Dim opens As Long

Private Sub mnuFile_Click()
    opens = opens + 1
    Label3.Caption = "File menu opened " & opens & " time(s)"
End Sub

Private Sub mnuNew_Click()
    Dim answer As Integer
    answer = MsgBox("Save changes?", vbYesNoCancel + vbQuestion, "New")
    Label2.Caption = "New: answer " & answer
End Sub

Private Sub mnuOpen_Click()
    Dim name As String
    name = InputBox("File name?", "Open", "a.txt")
    Label1.Caption = "Opened [" & name & "]"
End Sub

Private Sub mnuExit_Click()
    Unload Me
End Sub

Private Sub mnuBold_Click()
    mnuBold.Checked = Not mnuBold.Checked
    Label3.Caption = "Bold " & IIf(mnuBold.Checked, "on", "off")
End Sub

Private Sub mnuBlue_Click()
    Label1.Caption = "Blue"
End Sub
