#include <stdint.h>

struct idt_entry {
    uint16_t offset_low;    // Offset bits 0-15
    uint16_t selector;      // Selector
    uint8_t  zero;          // Unused, set to zero
    uint8_t  type_attr;     // Type and attributes
    uint16_t offset_high;   // Offset bits 16-31
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern void idt_load();     // Function in IDT.s to load the IDT

#define IDT_SIZE 256
struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;

void set_idt_entry(int n, uint32_t handler) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = 0x08;     // Kernel code segment
    idt[n].zero = 0;
    idt[n].type_attr = 0x8E;    // Interrupt gate
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

void init_idt() {
    idtp.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idtp.base = (uint32_t)&idt;

    // Set up IDT entries here
    // Example: set_idt_entry(33, (uint32_t)keyboard_interrupt_handler);

    // Load the IDT
    idt_load();
}

// Example keyboard interrupt handler
void keyboard_interrupt_handler() {
    // Handle keyboard interrupt
}

