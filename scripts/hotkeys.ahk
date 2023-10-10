#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.
#HotkeyModifierTimeout 0

Sendkey(key0) {
 SoundBeep, 1500, 20
 Send % "{" key0 "}"
}

KeyDown(key) {
 return % "{" key " Down}"
}

KeyDown2(key0, key1) {
 return % KeyDown(k0) "" KeyDown(k1)
}

KeyUp(key) {
 return % "{" key " Up}"
}

KeyUp2(key0, key1) {
 return KeyUp(k0) "" KeyUp(k1)
}

SendMultiKeys(k0, k1, k2) {
 SoundBeep, 1500, 20
 Send % KeyDown(k0) "" KeyDown(k1) "" KeyUp(k0) "" KeyUp(k1)
 if !(k2 = "") { 
  Send % "{" k2 " Up}" 
 }
}

Send3Keys(k0, k1, k2) {
  SoundBeep, 1500, 20
  Send % KeyDown(k0) "" KeyDown(k1) "" KeyDown(k2) "" KeyUp(k0) "" KeyUp(k1) "" KeyUp(k2)
}


;F1::SendMultiKeys("LWin", "Down", "LWin")
;F2::Sendkey("Volume_Down")
;F3::Sendkey("Volume_Up")
;F4::Sendkey("Volume_Mute")
;F5::SendMultiKeys("Shift", "<", "Media_Prev")
;F6::SendMultiKeys("Shift", ">", "Media_Next")
;F7::Sendkey("Media_Play_Pause")

;F3::SendMultiKeys("Shift", "<", "Media_Prev")
;F4::SendMultiKeys("Shift", ">", "Media_Next")
;F5::Sendkey("Left")
;F6::Sendkey("Right")
;F7::SendMultiKeys("Shift", "<", "Media_Prev")
;F8::Sendkey("Media_Play_Pause")
;F9::SendMultiKeys("Shift", ">", "Media_Next")
;F10::Sendkey("Volume_Mute")
;F11::Sendkey("Volume_Down")
;F12::Sendkey("Volume_Up")

~Ctrl & Left:: Sendkey("Home")
~Ctrl & Right:: Sendkey("End")
~Ctrl & `;:: Sendkey("End")

;MButton::Ctrl
;~MButton & `::Sendkey("Backspace")
;~MButton & Space::Sendkey("Enter")
;~MButton & Shift::SendMultiKeys("LWin", ";", "Shift")


;~Esc & F2::SendMultiKeys("LWin", "Down", "LWin")

~Esc & F2::Sendkey("Left")
~Esc & F3::Sendkey("Right")



~Rshift & a:: Sendkey("Home")
~RShift & s:: Sendkey("End")
~RShift & d:: Sendkey("Left")
~RShift & f:: Sendkey("Right")
~RShift & e:: Sendkey("Down")
~RShift & r:: Sendkey("Up")

~LWin & v::Backspace
~LWin & ]::Delete

~Alt & s:: SendMultiKeys("Ctrl", "s", "")
~Alt & a:: SendMultiKeys("Ctrl", "a", "")
~Alt & c:: SendMultiKeys("Ctrl", "c", "")
~Alt & v:: SendMultiKeys("Ctrl", "v", "")
~Alt & f:: Sendkey("Right")
~Alt & d:: Sendkey("Left")
~Alt & z:: SendMultiKeys("Ctrl", "z", "")
~Alt & t:: SendMultiKeys("Ctrl", "t", "")
~Alt & w:: SendMultiKeys("Ctrl", "w", "")
~Alt & Right:: SendMultiKeys("Ctrl", "Right", "")
~Alt & Left:: SendMultiKeys("Ctrl", "Left", "")

~LAlt & j:: Sendkey("{")
~LAlt & k:: Sendkey("}")

~LAlt & Space:: Sendkey("Delete")
~LAlt & LButton::Sendkey("Browser_Back")
~LAlt & RButton::Sendkey("Browser_Forward")
~LAlt & n:: Sendkey("Up")
~LAlt & m:: Sendkey("Down")

~RAlt & j:: Sendkey("Backspace")
~RAlt & k:: Sendkey("Delete")

#if GetKeyState("Alt", "P")
LShift & f:: Sendkey("End")
LShift & d:: Sendkey("Home")
#if


CapsLock::return
~CapsLock & 1::SendKey("<")
~CapsLock & 2::SendKey(">")
~CapsLock & q::SendKey("Left")
~CapsLock & w::SendKey("Right")
~CapsLock & a::SendKey("Up")
~CapsLock & s::SendKey("Down")
~CapsLock & `::
 T := !T
 if (T) {
  Send3Keys("Ctrl", "LWin", "Right")
 } else {
  Send3Keys("Ctrl", "LWin", "Left")
 }
 return


~MButton & a::SendMultiKeys("LWin", "Right", "")
~MButton & d::SendMultiKeys("LWin", "Left", "")
~MButton & w::SendMultiKeys("LWin", "Up", "")
~Mbutton & s::SendMultiKeys("LWin", "Down", "")
~Mbutton & `::
 T := !T
 if (T) {
  Send3Keys("Ctrl", "LWin", "Right")
 } else {
  Send3Keys("Ctrl", "LWin", "Left")
 }
 return


:*:rr::
 Send % "return "
 return

:*:uu32::
 Send % "uint32_t "
 return

:*:uu8::
 Send % "uint8_t "
 return

:*:SSK::
 Send % "Sendkey(""kk"")"
 return

:*:SSS::
 Send % "SendMultiKeys(""kk"", ""kk"", ""kk"")"
 return

:*:CLA::
 Send % "class "
 return 

:*:PPP::
 Send % "public "
 return 

:*:PPV::
 Send % "private "
 return

:*:VVV::
 Send % "void "
 return 

:*:BBB::
 Send % "bool "
 return 

:*:GKS::
 Send % "GetKeyState("""
 return