#include "cpu.h"
#include <memory.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_MEMORY 65535

byte memory[MAX_MEMORY];

byte read(uint16_t addr) { return memory[addr]; }

void write(byte m, uint16_t addr) { memory[addr] = m; }

int main(int argc, char **argv) {
    mos6502 *cpu = createCpu();
    cpu->Read = read;
    cpu->Write = write;
    char *binary_path = {0};
    if (argc >= 2) {
        binary_path = argv[1];
        FILE *code = fopen(binary_path, "rb");
        fread(memory, sizeof(memory), 1, code);
        fclose(code);
    } else {
        printf("Path to binary not provided!");
        return -1;
    }
    Run(cpu);
    return 0;
}
