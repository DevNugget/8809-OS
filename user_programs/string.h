#include <stddef.h>

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// strncmp
int strncmp(const char* s1, const char* s2, unsigned int n) {
    while (n && *s1 && (*s1 == *s2)) {
        ++s1; ++s2; --n;
    }
    return n == 0 ? 0 : *(unsigned char*)s1 - *(unsigned char*)s2;
}

char* strcpy(char* dest, const char* src) {
    char* original = dest;
    while ((*dest++ = *src++));
    return original;
}

int strlen(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char* strcat(char* dest, const char* src) {
    char* original = dest;

    // Move to the end of dest
    while (*dest != '\0') {
        dest++;
    }

    // Append src to dest
    while ((*dest++ = *src++));

    return original;
}

char* strchr(const char* s, int c) {
    while (*s) {
        if (*s == (char)c) return (char*)s;
        s++;
    }
    return NULL;
}

char *strtok(char *str, const char *delim) {
    static char *next;
    if (str)
        next = str;
    if (!next)
        return NULL;

    // Skip leading delimiters
    while (*next && strchr(delim, *next))
        next++;

    if (!*next)
        return NULL;

    char *start = next;

    // Find the end of this token
    while (*next && !strchr(delim, *next))
        next++;

    if (*next) {
        *next = '\0';
        next++;
    } else {
        next = NULL;
    }

    return start;
}

char *strncpy(char *dest, const char *src, unsigned int n) {
    unsigned int i = 0;

    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    if (i < n) {
        dest[i] = '\0'; // Ensure null-termination
    } else {
        dest[n - 1] = '\0'; // Force termination if we filled the buffer
    }

    return dest;
}

void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = (unsigned char*) dest;
    const unsigned char* s = (const unsigned char*) src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}
