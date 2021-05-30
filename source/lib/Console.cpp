#pragma once
#include "headers/Typedefs.h"
#include "textutils.cpp"
#include "IDT.cpp"
#include "sysutils.cpp"
#include "sysinfo.cpp"

#define INPUT_LENGTH 256


namespace CMD{
  char poweroff[INPUT_LENGTH] = "poweroff";
  char sysinfo[INPUT_LENGTH] = "sysinfo";
  char systests[INPUT_LENGTH] = "systests";
  char clear[INPUT_LENGTH] = "clear";
  char help[INPUT_LENGTH] = "help";
}

namespace ACTIONS{
  void help(){
    PrintString("--------------- HELP --------------\n\r");
    PrintString("help          show this help\n\r");
    PrintString("clear         clear the Console\n\r");
    PrintString("systests      Run systests\n\r");
    PrintString("sysinfo       show system info\n\r");
    PrintString("poweroff      poweroff the system\n\r");
  }
}


bool ReturnPressed = false;
bool LeftShiftPressed = false;
bool RightShiftPressed = false;
uint_8 LastScanCode;

int cmdPos = 0;
char command[INPUT_LENGTH];
char* cmdPtr = command;

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
      if (CurserPosition % VGA_WIDTH > 4){
        SetCursorPosition(CurserPosition - 1);
        PrintChar(' ');
        SetCursorPosition(CurserPosition - 1);
        cmdPos--;
        *(cmdPtr + cmdPos) = '\0';
      }
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
      ReturnPressed = true;
      break;
    }
  }
  LastScanCode = scanCode;
}

void ClearCMD(){
  for (int i = 0; i < INPUT_LENGTH; i++){
     cmdPtr[i] = '\0';
   }
   cmdPos = 0;
   return;
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
  if (compare(command, CMD::poweroff)){
    sysutils::shutdown();
  }
  else if (compare(command, CMD::sysinfo)){
    systemInfo();
  }
  else if (compare(command, CMD::systests)){
    systemTests();
  }
  else if (compare(command, CMD::clear)){
    ClearScreen();
    SetCursorPosition(0);
  }
  else if (compare(command, CMD::help)){
    ACTIONS::help();
  }
  else{
    PrintString("Command not found!");
  }
}

void Console(){
  MainKeyboardHandler = ConsoleKeyboardHandler;
  PrintString("CryptOS Console:");
  while (true) {
    PrintString("\n\r>>> ", BACKGROUND_BLACK | FOREGROUND_GREEN);
    while (!ReturnPressed){}
    ReturnPressed = false;
    CommandHandler();
    ClearCMD();
  }
}
