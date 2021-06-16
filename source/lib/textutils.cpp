#pragma once
//include libs
#include "IO.cpp"
//include headers
#include "headers/Typedefs.h"
#include "headers/TextModeColorCodes.h"
#include "headers/UI.h"
//definitions
#define VGA_MEMORY (uint_8*)0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define STD_COLOR BACKGROUND_BLACK | FOREGROUND_WHITE

#define BOTTOM_MARGIN 1
#define TOP_MARGIN 1
#define LEFT_MARGIN 0
#define RIGHT_MARGIN 0


struct coords{
  uint_8 x;
  uint_8 y;
};

uint_16 CursorPosition = 0;

uint_8 AUTO_SCROLL = 1;

char hexToStringOutput[128];
template<typename T>
const char* HexToString(T value){
  T* valPtr = &value;
  uint_8* ptr;
  uint_8 temp;
  uint_8 size = (sizeof(T)) * 2 - 1;
  uint_8 i;
  for (i = 0; i < size; i++){
    ptr = ((uint_8*)valPtr + i);
    temp = ((*ptr & 0xF0) >> 4);
    hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
    temp = ((*ptr & 0x0F));
    hexToStringOutput[size - (i * 2 + 0)] = temp + (temp > 9 ? 55 : 48);
  }
  hexToStringOutput[size + 1] = 0;
  return hexToStringOutput;
}

char integerToStringOutput[128];
template <typename T>
const char* IntegerToString(T value){
  uint_8 isNegative = 0;

  if (value < 0) {
    isNegative = 1;
    value *= -1;
    integerToStringOutput[0] = '-';
  }

  uint_8 size = 0;
  uint_64 sizeTester = (uint_64)value;
  while (sizeTester / 10 > 0) {
    sizeTester /= 10;
    size++;
  }

  uint_8 index = 0;
  uint_64 newValue = (uint_64)value;
  while (newValue / 10 > 0){
    uint_8 remainder = newValue % 10;
    newValue /= 10;
    integerToStringOutput[isNegative + size - index] = remainder + 48;
    index++;
  }
  uint_8 remainder = newValue % 10;
  integerToStringOutput[isNegative + size - index] = remainder + 48;
  integerToStringOutput[isNegative + size + 1] = 0;
  return integerToStringOutput;
}

char floatToStringOutput[128];
const char* FloatToString(float value, uint_8 decimalPlaces){
  char* intPtr = (char*)IntegerToString((int)value);
  char* floatPtr = floatToStringOutput;

  if (value < 0){
    value *= -1;
  }

  while (*intPtr != 0){
    *floatPtr = *intPtr;
    intPtr++;
    floatPtr++;
  }
  *floatPtr = '.';
  floatPtr++;

  float newValue = value - (int)value;

  for (uint_8 i = 0; i < decimalPlaces; i++){
    newValue *= 10;
    *floatPtr = (int)newValue + 48;
    newValue -= (int)newValue;
    floatPtr++;
  }

  *floatPtr = 0;

  return floatToStringOutput;
}

void ClearScreen(uint_64 ClearColor = STD_COLOR){
  uint_64 value = 0;
  value += ClearColor << 8;
  value += ClearColor << 24;
  value += ClearColor << 40;
  value += ClearColor << 56;
  for(uint_64* i = (uint_64*)VGA_MEMORY; i < (uint_64*)(VGA_MEMORY + 4000); i++){
    *i = value;
  }
}

namespace panic{
  void stop(){
    while (true){
      asm volatile ("hlt");
    }
  }
  coords CoordFromPosition(uint_16 position){
    coords crds;
    crds.x = position % VGA_WIDTH;
    crds.y = (int)(position / VGA_WIDTH);
    return crds;
  }
  void SetCursorPosition(uint_16 position){
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint_8)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint_8)((position >> 8) & 0xFF));
    CursorPosition = position;
  }
  void PrintChar(char chr, uint_8 color = STD_COLOR){
    switch (chr) {
      case 10:
        SetCursorPosition(CursorPosition + VGA_WIDTH);
        break;
      case 13:
        SetCursorPosition(CursorPosition-CoordFromPosition(CursorPosition).x);
        break;
      default:
        *(VGA_MEMORY + CursorPosition * 2) = chr;
        *(VGA_MEMORY + CursorPosition * 2 + 1) = color;
        SetCursorPosition(CursorPosition + 1);
    }
  }
  void PrintString(const char* str, uint_8 color = STD_COLOR){
    uint_8* charPtr = (uint_8*)str;
    while(*charPtr != 0){
        PrintChar(*charPtr, color);
        charPtr++;
    }
  }

  void PanicScreen(const char* exception){
    ClearScreen(BACKGROUND_RED | FOREGROUND_RED);
    SetCursorPosition(0);
    PrintString("KERNEL_PANIC!\n\r-------------\n\rEXCEPTION: ", BACKGROUND_RED | FOREGROUND_WHITE);
    PrintString(exception, BACKGROUND_RED | FOREGROUND_WHITE);

    stop();
  }
}

void SetCursorPosition(uint_16 position){
  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint_8)(position & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint_8)((position >> 8) & 0xFF));
  CursorPosition = position;
}

uint_16 PositionFromCoords(uint_8 x, uint_8 y){
  return y * VGA_WIDTH + x;
}

coords CoordFromPosition(uint_16 position){
  coords crds;
  crds.x = position % VGA_WIDTH;
  crds.y = (int)(position / VGA_WIDTH);
  return crds;
}

void Scroll(){
  int multiplicator = 2;
  for (int y = TOP_MARGIN; y < VGA_HEIGHT-BOTTOM_MARGIN; y++){
    for (int x = LEFT_MARGIN; x < VGA_WIDTH - RIGHT_MARGIN; x++){
      unsigned char chr = *(VGA_MEMORY + PositionFromCoords(x, y) * multiplicator + VGA_WIDTH*2 );
      unsigned int clr = *(VGA_MEMORY + PositionFromCoords(x, y) * multiplicator + 1 + VGA_WIDTH*2 );
      *(VGA_MEMORY + PositionFromCoords(x, y) * multiplicator) = chr;
      *(VGA_MEMORY + PositionFromCoords(x, y) * multiplicator + 1) = clr;
    }
  }
  SetCursorPosition(CursorPosition - VGA_WIDTH);
}

void PrintChar(char chr, uint_8 color = STD_COLOR){

  if (AUTO_SCROLL && CoordFromPosition(CursorPosition).y+1 > VGA_HEIGHT - BOTTOM_MARGIN){
    Scroll();
  }

  coords crds = CoordFromPosition(CursorPosition);

  switch (chr) {
    case 10:
      SetCursorPosition(CursorPosition + VGA_WIDTH);
      break;
    case 13:
      SetCursorPosition(CursorPosition-CoordFromPosition(CursorPosition).x);
      break;
    default:
      *(VGA_MEMORY + CursorPosition * 2) = chr;
      *(VGA_MEMORY + CursorPosition * 2 + 1) = color;
      SetCursorPosition(CursorPosition + 1);
    }
}

void PrintString(const char* str, uint_8 color = STD_COLOR){
  uint_8* charPtr = (uint_8*)str;
  while(*charPtr != 0){
      PrintChar(*charPtr, color);
      charPtr++;
  }
}