#pragma once
#include "Typedefs.h"
#include "Memory.h"

struct MemorySegmentHeader {
  uint_64 MemoryLength;
  MemorySegmentHeader* NextSegment;
  MemorySegmentHeader* PreviousSegment;
  MemorySegmentHeader* NextFreeSegment;
  MemorySegmentHeader* PreviousFreeSegment;
  bool free;
};

struct AlignedMemorySegmentHeader{
    uint_64 MemorySegmentHeaderAddress : 63;
    bool IsAligned : 1;
};

void InitializeHeap(uint_64 heapAddress, uint_64 HeapLength);

void* calloc(uint_64 size);

void* realloc(void* address, uint_64 newSize);

void* malloc(uint_64 size);

void* aligned_alloc(uint_64 allignment, uint_64 size);

void free(void* address);
