#include <stdint.h>

// Function prototypes
void outb(uint16_t port, uint8_t val);

void move_cursor(int row, int col) {
    // Calculate cursor position from row and col
    uint16_t pos = row * 80 + col;

    // Send the high byte of the position to the VGA control register
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);

    // Send the low byte of the position to the VGA control register
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

