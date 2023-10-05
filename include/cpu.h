#pragma once
#include "types.h"

typedef struct {

    byte A; // Accumulator
    byte X; // index Register X
    byte Y; // index Register Y
    byte status; // Status flags
    byte sp; // Stack pointer
    uint16_t pc; // Program counter
    Instruction table[256];
    ReadMem Read;
    WriteMem Write;

} mos6502;

void NMI();
void IRQ();
void Reset();

void Run();
void Execute(Instruction i);
