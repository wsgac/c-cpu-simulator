#include <stdio.h>
#include "cpu.h"

void fetch_instruction(struct CPU *cpu, unsigned char code[]);

void reset(struct CPU *cpu) {
    cpu->SP = 0;
    cpu->PC = 0;
    int i;
    for(i = 0; i < 16; i++)
        cpu->labels[i] = 0; 
    for(i = 0; i < 17; i++)
        cpu->registers[i] = 0;
    for(i = 0; i < STACKSIZE; i++)
        cpu->stack[i] = 0;
}

void preprocess(struct CPU *cpu, unsigned char code[], int code_length) {
    for(int i = 0; i < code_length; i++) {
        if ((code[i] & LABEL) == LABEL) {
            cpu->labels[(code[i] & 0x1e) >> 1] = i + 1;
        }
    }
}

int run(struct CPU *cpu, unsigned char code[], int code_length) {
    reset(cpu);
    preprocess(cpu, code, code_length);
    while (cpu->PC < code_length) {
        fetch_instruction(cpu, code);
    }
    return cpu->registers[0];
}

void fetch_instruction(struct CPU *cpu, unsigned char code[]) {
    unsigned char op, instruction = code[cpu->PC++];
    unsigned int val, reg, opt;
    op = instruction & LABEL;
	switch (op) {
	case SET: // SET
		val = instruction & VALUE;
		cpu->registers[0] = val;
        break;
	case ADD: // ADD
		reg = ((instruction & REG) >> 1) + 1;
		cpu->registers[0] += cpu->registers[reg];
        break;
	case SUB: // SUB
		reg = ((instruction & REG) >> 1) + 1;
		cpu->registers[0] -= cpu->registers[reg];
        break;
	case MUL: // MUL
		reg = ((instruction & REG) >> 1) + 1;
		cpu->registers[0] *= cpu->registers[reg];
        break;
	case PUSH: // PUSH
		opt = instruction & 0x01;
		if (opt == 1) {
			reg = 0;
		} else {
			reg = ((instruction & REG) >> 1) + 1;
		}
		cpu->stack[cpu->SP++] = cpu->registers[reg];
        break;
	case POP: // POP
		opt = instruction & 0x01;
		if (opt == 1) {
			reg = 0;
		} else {
			reg = ((instruction & REG) >> 1) + 1;
		}
		cpu->registers[reg] = cpu->stack[--cpu->SP];
        break;
	case GOTO: // GOTO
		opt = instruction & 0x01;
		if (opt == 1) { // R0 != 0
			if (cpu->registers[0] != 0) {
				cpu->PC = cpu->labels[(instruction & 0x1e) >> 1];
			}
		} else { // R0 == 0
			if (cpu->registers[0] == 0) {	
			    cpu->PC = cpu->labels[(instruction & 0x1e) >> 1];
			}
		}
        break;
	case LABEL: // LABEL
		break;
	}
}

void test_summation_1_to_10() {
    struct CPU cpu = {};
    unsigned char code[] = {
        0x00, 0x81, 0xa0, // INIT R1
		0x0a, 0x81, 0xa2, // INIT R2
		0x01, 0x81, 0xa4, // INIT R3
		0xe0, // LABEL 0
		0x80, 0xa1, 0x22, 0x81, 0xa0, 0x82, 0xa1, 0x44, 0x81, 0xa2,
		0xc1, // GOTO 0
		0x80, 0xa1
        // 0x00, 0x81, 0xa0, 0x0b, 0x81, 0xa2, 0x01, 0x81, 0xa4, 0xe0,
		// 0x80, 0xa1, 0x24, 0x81, 0xa0, 0x01, 0x24, 0x81, 0xa4, 0x42,
		// 0xc1, 0x80, 0xa1
    };
    int code_length = sizeof(code)/sizeof(unsigned char);
    printf("Preparing to test 1-to-10 summation [code length: %d]\n",
    code_length);
    unsigned int res = run(&cpu, code, code_length);
    if (res == 55) {
        printf("Result of 1-to-10 summation as expected: %d\n", res);
    } else {
        printf("Result of 1-to-10 summation wrong. Got: %d Expected: %d\n", res, 55);
    }
}

int main() {
    test_summation_1_to_10();
    return 0;
}
