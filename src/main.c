#include "mos6502.h"
#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#define MAX_MEMORY 65535

typedef byte (*ReadMem)(uint16_t addr);
typedef void (*WriteMem)(byte m,uint16_t addr);

byte memory[MAX_MEMORY];

byte read(uint16_t addr) {
    return memory[addr];
}

void write(byte m,uint16_t addr) {
    memory[addr] = m;
}

int main(void) {
    memset(memory,0xE8,MAX_MEMORY);
    mos6502* cpu = createCpu();
    cpu->Read = read;
    cpu->Write = write;
    Run(cpu);
    printf("cpu : %p",cpu);
    return 0;
}
