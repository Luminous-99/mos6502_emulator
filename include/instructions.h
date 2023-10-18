#pragma once
#include <stdint.h>
#include "cpu.h"

// Transfer Instructions
void LDA(uint16_t addr, mos6502* cpu);
void LDX(uint16_t addr, mos6502* cpu);
void LDY(uint16_t addr, mos6502* cpu);
void STA(uint16_t addr, mos6502* cpu);
void STX(uint16_t addr, mos6502* cpu);
void STY(uint16_t addr, mos6502* cpu);
void TAX(uint16_t addr, mos6502* cpu);
void TAY(uint16_t addr, mos6502* cpu);
void TSX(uint16_t addr, mos6502* cpu);
void TXA(uint16_t addr, mos6502* cpu);
void TXS(uint16_t addr, mos6502* cpu);
void TYA(uint16_t addr, mos6502* cpu);

// Stack instructions
void PHA(uint16_t addr, mos6502* cpu);
void PHP(uint16_t addr, mos6502* cpu);
void PLA(uint16_t addr, mos6502* cpu);
void PLP(uint16_t addr, mos6502* cpu);

// Increment & Decrement instructions
void DEC(uint16_t addr, mos6502* cpu);
void DEY(uint16_t addr, mos6502* cpu);
void DEX(uint16_t addr, mos6502* cpu);
void INC(uint16_t addr, mos6502* cpu);
void INY(uint16_t addr, mos6502* cpu);
void INX(uint16_t addr, mos6502* cpu);

// Arithmetic instructions
void ADC(uint16_t addr, mos6502* cpu);
void SBC(uint16_t addr, mos6502* cpu);

// Logical instructions
void AND(uint16_t addr, mos6502* cpu);
void ORA(uint16_t addr, mos6502* cpu);
void EOR(uint16_t addr, mos6502* cpu);

// Shift & Rotate instructions
void ASL(uint16_t addr, mos6502* cpu);
void ASL_A(uint16_t addr, mos6502* cpu);
void LSR(uint16_t addr, mos6502* cpu);
void LSR_A(uint16_t addr, mos6502* cpu);
void ROL(uint16_t addr, mos6502* cpu);
void ROL_A(uint16_t addr, mos6502* cpu);
void ROR(uint16_t addr, mos6502* cpu);
void ROR_A(uint16_t addr, mos6502* cpu);

// Flag instructions 
void CLC(uint16_t addr, mos6502* cpu);
void CLD(uint16_t addr, mos6502* cpu);
void CLI(uint16_t addr, mos6502* cpu);
void CLV(uint16_t addr, mos6502* cpu);
void SEC(uint16_t addr, mos6502* cpu);
void SED(uint16_t addr, mos6502* cpu);
void SEI(uint16_t addr, mos6502* cpu);

// Comparison instructions
void CMP(uint16_t addr, mos6502* cpu);
void CPX(uint16_t addr, mos6502* cpu);
void CPY(uint16_t addr, mos6502* cpu);

// Conditional Branch instructions
void BCC(uint16_t addr, mos6502* cpu);
void BCS(uint16_t addr, mos6502* cpu);
void BEQ(uint16_t addr, mos6502* cpu);
void BMI(uint16_t addr, mos6502* cpu);
void BNE(uint16_t addr, mos6502* cpu);
void BPL(uint16_t addr, mos6502* cpu);
void BVC(uint16_t addr, mos6502* cpu);
void BVS(uint16_t addr, mos6502* cpu);

// Jumps & Subroutines
void JMP(uint16_t addr, mos6502* cpu);
void JSR(uint16_t addr, mos6502* cpu);
void RTS(uint16_t addr, mos6502* cpu);

// Interrupts
void BRK(uint16_t addr, mos6502* cpu);
void RTI(uint16_t addr, mos6502* cpu);

// Others 
void BIT(uint16_t addr, mos6502* cpu);
void NOP(uint16_t addr, mos6502* cpu);
