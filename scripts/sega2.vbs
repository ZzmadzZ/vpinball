'Last Updated in VBS v3.56

' GoldenEye and Apollo13 use different flipper techinques and switch locations than other Sega games.

Option Explicit
LoadCore
Private Sub LoadCore
	On Error Resume Next
	If VPBuildVersion < 0 Or Err Then
		Dim fso : Set fso = CreateObject("Scripting.FileSystemObject") : Err.Clear
		ExecuteGlobal fso.OpenTextFile("core.vbs", 1).ReadAll    : If Err Then MsgBox "Can't open ""core.vbs""" : Exit Sub
		ExecuteGlobal fso.OpenTextFile("VPMKeys.vbs", 1).ReadAll : If Err Then MsgBox "Can't open ""vpmkeys.vbs""" : Exit Sub
	Else
		ExecuteGlobal GetTextFile("core.vbs")    : If Err Then MsgBox "Can't open ""core.vbs"""    : Exit Sub
		ExecuteGlobal GetTextFile("VPMKeys.vbs") : If Err Then MsgBox "Can't open ""vpmkeys.vbs""" : Exit Sub
	End If
End Sub

'----------------------------
' Apollo13 / GoldenEye Data
'----------------------------
' Cabinet switches
Const swBlack          =  0 'DED 8
Const swGreen          = -1 'DED 7
Const swRed            = -2 'DED 6
Const swStartButton    = 3
Const swTilt           = 1
Const swSlamTilt       = 8
Const swCoin3          = 4
Const swCoin1          = 5
Const swCoin2          = 6

Const swLRFlip         = 64
Const swLLFlip         = 63

Const GameOnSolenoid = 48
'This game requires UseSolenoids = 2 for proper flipper activation
Const UseSolenoids = 2

' Help Window
vpmSystemHelp = "Sega keys:" & vbNewLine &_
  vpmKeyName(keyInsertCoin1)  & vbTab & "Insert Coin #1"   & vbNewLine &_
  vpmKeyName(keyInsertCoin2)  & vbTab & "Insert Coin #2"   & vbNewLine &_
  vpmKeyName(keyInsertCoin3)  & vbTab & "Insert Coin #3"   & vbNewLine &_
  vpmKeyName(keyBlack)        & vbTab & "Black"            & vbNewLine &_
  vpmKeyName(keyGreen)        & vbTab & "Green"            & vbNewLine &_
  vpmKeyName(keyRed)          & vbTab & "Red"              & vbNewLine &_
  vpmKeyName(keySlamDoorHit)  & vbTab & "Slam Tilt"

'Dip Switch / Options Menu
Private Sub sega2ShowDips
	If Not IsObject(vpmDips) Then ' First time
		Set vpmDips = New cvpmDips
		With vpmDips
			.AddForm 100, 240, "DIP Switches"
			.AddFrame 0, 0, 80, "Country", &H0f,_
			  Array("Austria",&H01, "Belgium", &H02, "Brazil",      &H0d,_
			        "Canada", &H03, "France",  &H06, "Germany",     &H07,_
			        "Italy",  &H08, "Japan",   &H09, "Netherlands", &H04,_
			        "Norway", &H0a, "Sweden",  &H0b, "Switzerland", &H0c,_
			        "UK",     &H05, "UK (New)",&H0e, "USA",         &H00)
		End With
	End If
	vpmDips.ViewDips
End Sub
Set vpmShowDips = GetRef("sega2ShowDips")
Private vpmDips
' Keyboard handlers
Function vpmKeyDown(ByVal keycode)
	On Error Resume Next
	vpmKeyDown = True ' Assume we handle the key
	With Controller
		Select Case keycode
			Case RightFlipperKey .Switch(swLRFlip) = True : vpmKeyDown = False : vpmFlips.FlipR True
			Case LeftFlipperKey  .Switch(swLLFlip) = True : vpmKeyDown = False : vpmFlips.FlipL True
			Case keyInsertCoin1  vpmTimer.AddTimer 750,"vpmTimer.PulseSw swCoin1'" : Playsound SCoin
			Case keyInsertCoin2  vpmTimer.AddTimer 750,"vpmTimer.PulseSw swCoin2'" : Playsound SCoin
			Case keyInsertCoin3  vpmTimer.AddTimer 750,"vpmTimer.PulseSw swCoin3'" : Playsound SCoin
			Case StartGameKey    .Switch(swStartButton)  = True
			Case keyBlack        .Switch(swBlack)        = True
			Case keyGreen        .Switch(swGreen)        = True
			Case keyRed          .Switch(swRed)          = True
			Case keySlamDoorHit  .Switch(swSlamTilt)     = True
			Case keyBangBack     vpmNudge.DoNudge 0, 6
			Case LeftTiltKey     vpmNudge.DoNudge 75, 2
			Case RightTiltKey    vpmNudge.DoNudge 285, 2
			Case CenterTiltKey   vpmNudge.DoNudge 0, 2
			Case keyVPMVolume    vpmVol
			Case Else            vpmKeyDown = False
		End Select
	End With
	On Error Goto 0
End Function

Function vpmKeyUp(ByVal keycode)
	On Error Resume Next
	vpmKeyUp = True ' Assume we handle the key
	With Controller
		Select Case keycode
			Case RightFlipperKey .Switch(swLRFlip) = False : vpmKeyUp = False : vpmFlips.FlipR False
			Case LeftFlipperKey  .Switch(swLLFlip) = False : vpmKeyUp = False : vpmFlips.FlipL False
			Case StartGameKey    .Switch(swStartButton)  = False
			Case keyBlack        .Switch(swBlack)        = False
			Case keyGreen        .Switch(swGreen)        = False
			Case keyRed          .Switch(swRed)          = False
			Case keySlamDoorHit  .Switch(swSlamTilt)     = False
			Case keyShowOpts     .Pause = True : vpmShowOptions : .Pause = False
			Case keyShowKeys     .Pause = True : vpmShowHelp : .Pause = False
			Case keyShowDips     If IsObject(vpmShowDips) Then .Pause = True : vpmShowDips : .Pause = False
			Case keyAddBall      .Pause = True : vpmAddBall  : .Pause = False
			Case keyReset        .Stop : BeginModal : .Run : vpmTimer.Reset : EndModal
			Case keyFrame        .LockDisplay = Not .LockDisplay
			Case keyDoubleSize   .DoubleSize  = Not .DoubleSize
			Case Else            vpmKeyUp = False
		End Select
	End With
	On Error Goto 0
End Function
