section .text
global idt_load

; IDT setup
idt_load:
    mov eax, idt_ptr           ; Load address of IDT pointer into eax
    lidt [eax]                 ; Load IDT
    ret

; IDT entries (256 entries for all interrupts)
section .bss
align 16
idt_ptr:  dw idt_end - idt - 1 ; IDT limit (size - 1)
         dd idt                ; IDT base address
idt:      resb 256 * 8         ; Reserve space for 256 IDT entries (8 bytes each)
idt_end:

