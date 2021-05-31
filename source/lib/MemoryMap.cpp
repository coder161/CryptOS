#pragma once
#include "headers/Typedefs.h"
#include "textutils.cpp"

struct MemoryMapEntry{
  uint_64 BaseAddress;
  uint_64 RegionLength;
  uint_32 RegionType;
  uint_32 ExtendedAttributes;
};

extern uint_8 MemoryRegionCount;
uint_8 UsableMemoryRegionCount;

MemoryMapEntry* UsableMemoryRegions[10];

void PrintMemoryMap(MemoryMapEntry* memoryMap, uint_16 position){
  SetCursorPosition(position);

  PrintString("Memory Base: "); PrintString(IntegerToString(memoryMap->BaseAddress));
  PrintString("\n\rRegion Length: "); PrintString(IntegerToString(memoryMap->RegionLength));
  PrintString("\n\rMemory Type: "); PrintString(IntegerToString(memoryMap->RegionType));
  PrintString("\n\rMemory Attributes: "); PrintString(HexToString(memoryMap->ExtendedAttributes));
  PrintString("\n\n\r");
}

bool MemoryRegionsGot = false;

MemoryMapEntry** GetUsableMemoryRegions() {

  if (MemoryRegionsGot){
    return UsableMemoryRegions;
  }

  uint_8 UsableRegionIndex;
  for (uint_8 i = 0; i < MemoryRegionCount; i++){
    MemoryMapEntry* memMap = (MemoryMapEntry*)0x5000;
    memMap += i;
    if (memMap->RegionType == 1){
      UsableMemoryRegions[UsableRegionIndex] = memMap;
      UsableRegionIndex++;
    }
  }
  UsableMemoryRegionCount = UsableRegionIndex;

  MemoryRegionsGot = true;
  return UsableMemoryRegions;
}
