#pragma once
#include "types.h"

void NMI(mos6502* cpu);
void IRQ(mos6502* cpu);
void Reset(mos6502* cpu);

byte Pop(mos6502* cpu);
void Push(byte m, mos6502* cpu);
mos6502* createCpu();

void Run(mos6502* cpu);
void Execute(Instruction i,mos6502* cpu);
