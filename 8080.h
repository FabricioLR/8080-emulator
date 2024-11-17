#ifndef _H_8080
#define _H_8080

#include <stdio.h>

extern unsigned char memory[0x10000];

extern __uint16_t pc;
extern __uint16_t sp;
extern __uint8_t a;
extern __uint8_t b;
extern __uint8_t c;
extern __uint8_t d;
extern __uint8_t e;
extern __uint8_t h;
extern __uint8_t l;

extern __uint8_t CF;
extern __uint8_t PF;
extern __uint8_t AF;
extern __uint8_t ZF;
extern __uint8_t SF;

extern int int_enable;
extern int cycles;
extern int whichInterrupt;

int emulate();

#endif

