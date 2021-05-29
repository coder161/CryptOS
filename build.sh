#!/bin/sh
set -e
echo "[build.sh] Compiling 'source/Sector 1/bootloader.asm'"
nasm source/Sector\ 1/bootloader.asm -f bin -o build/bootloader.bin
echo "[build.sh] Compiled"
echo "[build.sh] Compiling 'source/Sector 2+/extendedbootloader.asm'"
nasm source/Sector\ 2+/extendedbootloader.asm -f elf64 -o build/extendedbootloader.o
echo "[build.sh] Compiled"
echo "[build.sh] Compiling 'source/Sector 2+/bins.asm'"
nasm source/Sector\ 2+/bins.asm -f elf64 -o build/bins.o
echo "[build.sh] Compiled"

echo "[build.sh] Compiling 'source/kernel.cpp'"
compiler/bin/x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "source/kernel.cpp" -o "build/kernel.o"
echo "[build.sh] Compiled"

echo "[build.sh] Linking"
compiler/bin/x86_64-elf-ld -T"link.ld"
echo "[build.sh] Linked"

echo "[build.sh] Creating 'CryptOS.img'"
cat build/bootloader.bin build/kernel.bin > CryptOS.img
echo "[build.sh] Building Finished"

echo "[build.sh] Starting 'run.sh'"
bash run.sh
echo "[build.sh] All Done (:"
