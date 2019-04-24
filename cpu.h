#include <stdio.h>

#define STACKSIZE 1000

// CPU operation masks
#define	SET   0x00
#define	ADD   0x20
#define	SUB   0x40
#define	MUL   0x60
#define	PUSH  0x80
#define	POP   0xa0
#define	GOTO  0xc0
#define	LABEL 0xe0

// Helper masks
#define VALUE 0x1f
#define REG   0x1e

struct CPU
{
    int registers[17];
    int labels[16];
    int PC;
    int stack[STACKSIZE];
    int SP; 
};
