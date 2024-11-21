#include <stdint.h>
#include <stddef.h>
#include "string.h"
#include "cursor.h"

// Simulated file system (for demonstration purposes)
const char *files[] = {
    "file1.txt",
    "file2.txt",
    "file3.txt"
};
const int file_count = 3;

// Function prototypes
void print(const char *message);
void clear_screen();
void handle_command(const char *command);
void shutdown();
void outw(uint16_t port, uint16_t val);
extern void outb(uint16_t port, uint8_t val);
void itoa(int num, char *str, int base);
extern void move_cursor(int row, int col);  // Declare move_cursor as external

extern void init_idt();
extern uint8_t read_scancode();
extern char scancode_to_ascii(uint8_t scancode);

// Global cursor position
int row = 0;
int col = 0;

void kernel_main() {
    init_idt();
    clear_screen();
    
    print("Welcome to Fedurux Command-Line!\n> ");
    
    char command[256];
    int command_index = 0;

    while (1) {
        uint8_t scancode = read_scancode();
        char key = scancode_to_ascii(scancode);

        if (scancode == 0x1C) {  // Enter key
            command[command_index] = '\0';
            handle_command(command);
            print("> ");  // Print prompt again
            command_index = 0;
        } else if (scancode == 0x0E) {  // Backspace key
            if (command_index > 0) {
                command_index--;
                print("\b \b");  // Erase character on screen
            }
        } else if (key) {
            command[command_index++] = key;
            char str[2] = {key, '\0'};
            print(str);
        }
    }
}

void handle_command(const char *command) {
    if (strcmp(command, "clear") == 0) {
        clear_screen();
        print("Welcome to Fedurux Command-Line!\n> ");
    } else if (strcmp(command, "help") == 0) {
        print("\nCommands: clear, help, ls, echo, shutdown\n> ");
    } else if (strcmp(command, "ls") == 0) {
        print("\n");
        for (int i = 0; i < file_count; i++) {
            print(files[i]);
            print("\n");
        }
        print("> ");
    } else if (strncmp(command, "echo ", 5) == 0) {
        print("\n");
        print(command + 5);
        print("\n> ");
    } else if (strcmp(command, "shutdown") == 0) {
        print("\nShutting down...\n> ");
        shutdown();
    } else {
        print("\nUnknown command\n> ");
    }
}

void clear_screen() {
    char *video_memory = (char *)0xb8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = 0x07;
    }
    row = 0; // Reset cursor row
    col = 0; // Reset cursor column
    move_cursor(row, col);
}

void print(const char *message) {
    char *video_memory = (char *)0xb8000;

    for (int i = 0; message[i] != '\0'; i++) {
        if (message[i] == '\n') {
            row++;
            col = 0;
        } else if (message[i] == '\b') {
            if (col > 0) {
                col--;
                video_memory[(row * 80 + col) * 2] = ' ';
            }
        } else {
            video_memory[(row * 80 + col) * 2] = message[i];
            video_memory[(row * 80 + col) * 2 + 1] = 0x07;
            col++;
            if (col >= 80) {
                col = 0;
                row++;
            }
        }

        if (row >= 25) {
            // Scroll the screen up if we reach the bottom
            for (int y = 1; y < 25; y++) {
                for (int x = 0; x < 80; x++) {
                    video_memory[((y - 1) * 80 + x) * 2] = video_memory[(y * 80 + x) * 2];
                    video_memory[((y - 1) * 80 + x) * 2 + 1] = video_memory[(y * 80 + x) * 2 + 1];
                }
            }
            // Clear the last row
            for (int x = 0; x < 80; x++) {
                video_memory[((25 - 1) * 80 + x) * 2] = ' ';
                video_memory[((25 - 1) * 80 + x) * 2 + 1] = 0x07;
            }
            row = 24;
        }
    }

    move_cursor(row, col);
}

void shutdown() {
    // Use ACPI to shut down
    outw(0xB004, 0x2000);  // This is one method; your actual method may vary depending on the system.
}

void outw(uint16_t port, uint16_t val) {
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

void itoa(int num, char *str, int base) {
    int i = 0;
    int is_negative = 0;

    // Handle 0 explicitly, otherwise empty string is printed for 0
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // In standard itoa(), negative numbers are handled only with base 10
    // Otherwise, numbers are considered unsigned
    if (num < 0 && base == 10) {
        is_negative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (is_negative)
        str[i++] = '-';

    str[i] = '\0';  // Append null character

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}


