#include "lib/textutils.cpp"
#include "lib/sysinfo.cpp"
#include "lib/Console.cpp"

#include "lib/headers/TextModeColorCodes.h"
#include "lib/headers/Heap.h"
//#include "lib/headers/task.h"

extern const char Test[];


void main(){
  PrintString("Welcome to CryptOS\n\r");
  Console();
  return;
}

extern "C" void _start(){
  ClearScreen();
  SetCursorPosition(0);
  PrintString("[+] Starting Kernel");
  //initTasking();
  PrintString(".");
  InitializeIDT();
  PrintString(".");
  InitializeHeap(0x100000, 0x100000);
  PrintString(".");
  PrintString("\n\n\r");

  //systemTests();
  //systemInfo(Test);

  PrintString("\n\r");

  main();
  return;
}
