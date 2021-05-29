[org 0x7c00]

mov [BOOT_DISK], dl

mov bp, 0x7c00
mov sp, bp

call ReadDisk

jmp PROGRAM_SPACE

%include "source/Sector 1/print.asm"
%include "source/Sector 1/diskread.asm"

times 510 - ($ - $$) db 0

dw 0xaa55