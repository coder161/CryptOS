#pragma once

void systemTests(){
  PrintString("\n---------- SYSTEM TESTS START ----------");
  PrintString("\n\rHEX_TO_STR_TEST(1234ABCD): ");
  PrintString(HexToString(0x1234abcd));
  PrintString("\n\rCOLOR_TEST: ");
  PrintString("RED ", BACKGROUND_RED | FOREGROUND_BLACK);
  PrintString("BLUE ", BACKGROUND_BLUE | FOREGROUND_BLACK);
  PrintString("CYAN ", BACKGROUND_CYAN | FOREGROUND_BLACK);
  PrintString("BROWN  \n\r", BACKGROUND_BROWN | FOREGROUND_BLACK);
  PrintString("            ");
  PrintString("GREEN", BACKGROUND_GREEN | FOREGROUND_BLACK);
  PrintString("MAGENTA", BACKGROUND_MAGENTA | FOREGROUND_BLACK);
  PrintString("LIGHTGRAY", BACKGROUND_LIGHTGRAY | FOREGROUND_BLACK);
  PrintString("\n\r----------  SYSTEM TESTS END  ----------\n\n\r");
}
void systemInfo(const char* version = "0.0.1"){
  PrintString("\nSYSTEM INFO:");
  PrintString("\n\r    VERSION: ");
  PrintString(version);
  PrintString("\n\r    SCREEN_WIDTH: ");
  PrintString(IntegerToString(VGA_WIDTH));
  PrintString("\n\r    [*] Thanks to 'Poncho' on youtube.com <3");
  PrintString("\n\n\r");
}
