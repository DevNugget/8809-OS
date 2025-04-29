#ifndef STRING_H
#define STRING_H

#include <stddef.h>

void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);
unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
int memcmp(const void* s1, const void* s2, size_t n);
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
size_t strlen(const char* s);
char* strchr(const char* s, int c);

#endif
