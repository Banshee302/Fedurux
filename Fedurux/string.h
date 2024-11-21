#ifndef STRING_H
#define STRING_H

#include <stddef.h>

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

int strncmp(const char *str1, const char *str2, size_t n) {
    while (n && *str1 && (*str1 == *str2)) {
        ++str1;
        ++str2;
        --n;
    }
    if (n == 0) {
        return 0;
    } else {
        return (*(unsigned char *)str1 - *(unsigned char *)str2);
    }
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

#endif // STRING_H

