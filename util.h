#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stddef.h>

uint8_t inportb (unsigned short _port);
uint16_t inportw (uint16_t _port);
void outportb (unsigned short _port, unsigned char _data);
void outportw(uint16_t port, uint16_t data);

//void *memcpy(void *dest, const void *src, size_t count);
//void *memset(void *dest, char val, size_t count);
//nsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
//size_t strlen(const char *str);

#endif
