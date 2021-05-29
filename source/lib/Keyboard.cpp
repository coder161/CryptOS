#pragma once
#include "headers/Typedefs.h"
#include "textutils.cpp"

bool LeftShiftPressed = false;
bool RightShiftPressed = false;
uint_8 LastScanCode;

void StandardKeyboardHandler(uint_8 scanCode, uint_8 chr){
  if(chr != 0){
    switch (LeftShiftPressed | RightShiftPressed) {
      case true:
        PrintChar(chr - 32);
        break;
      case false:
        PrintChar(chr);
        break;
    }
  }
  else {
    switch (scanCode) {
    case 0x8E: //Backspace
      SetCursorPosition(CurserPosition - 1);
      PrintChar(' ');
      SetCursorPosition(CurserPosition - 1);
      break;
    case 0x2A: //Left Shift Pressed
      LeftShiftPressed = true;
      break;
    case 0xAA: //Left Shift Released
      LeftShiftPressed = false;
      break;
    case 0x36: //Right Shift Pressed
      RightShiftPressed = true;
      break;
    case 0xB6: //Right Shift Released
      RightShiftPressed = false;
      break;
    case 0x9C: //Enter
      PrintString("\n\r");
      break;
    }
  }
}

void KeyboardHandler0x0E(uint_8 scanCode){
  switch (scanCode) {
    case 0x50: //Cursor Down
      SetCursorPosition(CurserPosition + VGA_WIDTH);
      break;
    case 0x48: //Cursor Up
      SetCursorPosition(CurserPosition - VGA_WIDTH);
      break;
    case 0x4D: //Cursor Right
      SetCursorPosition(CurserPosition + 1);
      break;
    case 0x4B: //Cursor Left
      SetCursorPosition(CurserPosition - 1);
      break;
    case 0x49: //Page Up
      SetCursorPosition(CurserPosition - VGA_WIDTH * 5);
      break;
    case 0x51: //Page Down
      SetCursorPosition(CurserPosition + VGA_WIDTH * 5);
      break;
    default:
      break;
  }
}

void KeyboardHandler(uint_8 scanCode, uint_8 chr){
  switch (LastScanCode) {
    case 0xE0:
      KeyboardHandler0x0E(scanCode);
    default:
      StandardKeyboardHandler(scanCode, chr);
  }
  LastScanCode = scanCode;
}
