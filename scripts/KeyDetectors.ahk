#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

#UseHook
MsgBox, Press the FN key (and other keys if you wish) then click OK to see key history.`n`nAfter viewing the key history, press Esc to exit script.
KeyHistory
return

Esc::ExitApp