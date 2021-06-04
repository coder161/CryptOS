#pragma once
#include "headers/Typedefs.h"
#include "headers/Heap.h"
#include "textutils.cpp"
#include "IDT.cpp"
#include "sysutils.cpp"
#include "sysinfo.cpp"
#include "time.cpp"

#define INPUT_LENGTH 256

bool ReturnPressed = false;
bool LeftShiftPressed = false;
bool RightShiftPressed = false;
uint_8 LastScanCode;

int oldsecond = 0;

int cmdPos = 0;
char command[INPUT_LENGTH];
char* cmdPtr = command;

char history[1024][INPUT_LENGTH];
int historyPos = 0;
int historyGetPos = 0;

char info[VGA_WIDTH];
char* infoPtr = info;
int infoPos = 0;

void ClearCMD(){
  for (int i = 0; i < INPUT_LENGTH; i++){
     cmdPtr[i] = '\0';
   }
   cmdPos = 0;
   return;
}

void UpdateInfo(){
    uint_16 oldPos = CursorPosition;
    SetCursorPosition(PositionFromCoords(0, VGA_HEIGHT - 1));
    AUTO_SCROLL = 0;
    for (int i = 0; i < VGA_WIDTH; i++){
        PrintChar(' ', BACKGROUND_BLUE | FOREGROUND_WHITE);
    }
    SetCursorPosition(CursorPosition - VGA_WIDTH);
    for (int i = 0; i < VGA_WIDTH; i++){
        PrintChar(*(infoPtr + i), BACKGROUND_BLUE | FOREGROUND_WHITE);
    }
    AUTO_SCROLL = 1;
    SetCursorPosition(oldPos);
}

void SetInfo(int pos, char* text, int endpos=VGA_WIDTH){
    for (int i = pos; i < endpos; i++){
        *(infoPtr + i) = *text;
        text++;
    }
}

void update_time(){
    read_rtc();
    if ((int)second != oldsecond){
        SetInfo(43, (char*)IntegerToString(hour), 45);
        SetInfo(46, (char*)IntegerToString(minute), 48);
        SetInfo(49, (char*)IntegerToString(second), 51);
    }
    oldsecond = second;
}

namespace ACTIONS{
  void help(){
    PrintString("--------------- HELP --------------\n\r");
    PrintString("help          show this help\n\r");
    PrintString("clear         clear the Console\n\r");
    PrintString("systests      Run systests\n\r");
    PrintString("sysinfo       show system info\n\r");
    PrintString("poweroff      poweroff the system\n\r");
    PrintString("debug         xD\n\r");
    PrintString("memmaps       Print Memory Maps\n\r");
  }
  void debug(){
      update_time();

      return;
  }

  void memmaps(){
    MemoryMapEntry** UsableMemoryMaps = GetUsableMemoryRegions();

    for (uint_8 i = 0; i < UsableMemoryRegionCount; i++){
      MemoryMapEntry* memMap = UsableMemoryMaps[i];
      memMap += i;
      PrintMemoryMap(memMap, CursorPosition);
    }
  }
}

namespace KEYS{
    void Backspace(){
        if (CoordFromPosition(CursorPosition).x > 4){
          SetCursorPosition(CursorPosition - 1);
          PrintChar(' ');
          SetCursorPosition(CursorPosition - 1);
          cmdPos--;
          *(cmdPtr + cmdPos) = '\0';
        }
    }
    void LeftShift(bool pressed=true){
        LeftShiftPressed = pressed;
    }
    void RightShift(bool pressed=true){
        RightShiftPressed = pressed;
    }
    void Return(){
        PrintString("\n\r");
        ReturnPressed = true;
        char* ptr = command;
        historyPos++;
        historyGetPos = historyPos;
        for (int i = 0; i < INPUT_LENGTH; i++){
            history[historyPos][i] = *(ptr + i);
        }
        SetInfo(infoPos, (char*)IntegerToString(historyGetPos));
        UpdateInfo();
    }
    void Up(){
        while (!(CoordFromPosition(CursorPosition).x <= 4)){KEYS::Backspace();}
        ClearCMD();
        if (historyGetPos > 0){
            historyGetPos--;
            PrintString(history[historyGetPos]);
            char* ptr = history[historyGetPos];
            for (int i = 0; i < INPUT_LENGTH; i++){
                command[i] = *(ptr + i);
            }
        }
        SetInfo(infoPos, (char*)IntegerToString(historyGetPos));
        UpdateInfo();
    }
    void Down(){
        while (!(CoordFromPosition(CursorPosition).x <= 4)){KEYS::Backspace();}
        ClearCMD();
        if (historyGetPos < historyPos){
            historyGetPos++;
            PrintString(history[historyGetPos]);
            char* ptr = history[historyGetPos];
            for (int i = 0; i < INPUT_LENGTH; i++){
                command[i] = *(ptr + i);
            }
        }
        SetInfo(infoPos, (char*)IntegerToString(historyGetPos));
        UpdateInfo();
    }
}

void ConsoleKeyboardHandler(uint_8 scanCode, uint_8 chr){
  if(chr != 0){
    switch (LeftShiftPressed | RightShiftPressed) {
      case true:
        PrintChar(chr - 32);
        break;
      case false:
        PrintChar(chr);
        break;
    }
    *(cmdPtr + cmdPos) = chr;
    cmdPos++;
  }
  else {
    switch (scanCode) {
    case 0x8E: //Backspace
      KEYS::Backspace();
      break;
    case 0x2A: //Left Shift Pressed
      KEYS::LeftShift();
      break;
    case 0xAA: //Left Shift Released
      KEYS::LeftShift(false);
      break;
    case 0x36: //Right Shift Pressed
      KEYS::RightShift();
      break;
    case 0xB6: //Right Shift Released
      KEYS::RightShift(false);
      break;
    case 0x9C: //Enter
      KEYS::Return();
      break;
    case 0x50: //Cursor Down
      KEYS::Down();
      break;
    case 0x48: //Cursor Up
      KEYS::Up();
      break;
    }
  }
  LastScanCode = scanCode;
}

int compare(char a[],char b[]){
    int flag=0,i=0;  // integer variables declaration
    while(a[i]!='\0' &&b[i]!='\0')  // while loop
    {
       if(a[i]!=b[i])
       {
           flag=1;
           break;
       }
       i++;
    }
    if(flag==0)
    return 1;
    else
    return 0;
}

void CommandHandler(){
  PrintString("CryptOS: executing: ");
  PrintString(command);
  PrintString("\n\r");
  if (compare(command,(char*)"poweroff")){
    sysutils::shutdown();
  }
  else if (compare(command, (char*)"sysinfo")){
    systemInfo();
  }
  else if (compare(command, (char*)"systests")){
    systemTests();
  }
  else if (compare(command, (char*)"clear")){
    ClearScreen();
    SetCursorPosition(0);
  }
  else if (compare(command, (char*)"help")){
    ACTIONS::help();
  }
  else if (compare(command, (char*)"debug")){
    ACTIONS::debug();
  }
  else if (compare(command, (char*)"memmaps")){
    ACTIONS::memmaps();
  }
  else{
    PrintString("Command not found!");
  }
}

void Console(){
  MainKeyboardHandler = ConsoleKeyboardHandler;
  PrintString("CryptOS Console:");
  SetInfo(0, (char*)"MODE: Console | SYS_VERSION: 0.0.1 | TIME: 00:00:00 | HISTORY: 0");
  infoPos = 63;

  while (true) {
    PrintString("\n\r>>> ", BACKGROUND_BLACK | FOREGROUND_GREEN);

    UpdateInfo();

    while (!ReturnPressed){}
    ReturnPressed = false;
    CommandHandler();
    ClearCMD();
  }
}
