section .multiboot
    align 4
    dd 0x1BADB002              ; Magic number
    dd 0x00                    ; Flags
    dd -(0x1BADB002 + 0x00)    ; Checksum

section .text
global start
extern kernel_main

start:
    cli                        ; Clear interrupts

    ; Set up stack
    mov esp, stack_space + stack_size

    ; Call kernel main function
    call kernel_main

halt:
    hlt                        ; Halt the CPU
    jmp halt                   ; Infinite loop to prevent return

section .bss
align 16
stack_space:
    resb 8192                  ; 8KB stack space
stack_size equ $ - stack_space

