jmp EnterProtectedMode

%include "source/Sector 2+/gdt.asm"
%include "source/Sector 1/print.asm"

EnterProtectedMode:
    call EnableA20
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp codeseg:StartProtectedMode

EnableA20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

[bits 32]
%include "source/Sector 2+/cpuID.asm"
%include "source/Sector 2+/SimplePaging.asm"


StartProtectedMode:

    mov ax, dataseg
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call DetectCPUID
    call DetectLongMode
    call SetUpIdentityPaging
    call EditGDT
    jmp codeseg:Start64Bit

[bits 64]
[extern _start]

%include "source/Sector 2+/IDT.asm"

Start64Bit:
    call _start
    jmp $

ActivateSSE:
    ret
