#pragma once
#include <stdint.h>

// Transfer Instructions
void LDA(uint16_t addr);
void LDX(uint16_t addr);
void LDY(uint16_t addr);
void STA(uint16_t addr);
void STX(uint16_t addr);
void STY(uint16_t addr);
void TAX(uint16_t addr);
void TAY(uint16_t addr);
void TSX(uint16_t addr);
void TXA(uint16_t addr);
void TXS(uint16_t addr);
void TYA(uint16_t addr);

// Stack instructions
void PHA(uint16_t addr);
void PHP(uint16_t addr);
void PLA(uint16_t addr);
void PLP(uint16_t addr);

// Increment & Decrement instructions
void DEC(uint16_t addr);
void DEY(uint16_t addr);
void DEX(uint16_t addr);
void INC(uint16_t addr);
void INY(uint16_t addr);
void INX(uint16_t addr);

// Arithmetic instructions
void ADC(uint16_t addr);
void SBC(uint16_t addr);

// Logical instructions
void AND(uint16_t addr);
void ORA(uint16_t addr);
void EOR(uint16_t addr);

// Shift & Rotate instructions
void ASL(uint16_t addr);
void ASL_A(uint16_t addr);
void LSR(uint16_t addr);
void LSR_A(uint16_t addr);
void ROL(uint16_t addr);
void ROL_A(uint16_t addr);
void ROR(uint16_t addr);
void ROR_A(uint16_t addr);

// Flag instructions 
void CLC(uint16_t addr);
void CLD(uint16_t addr);
void CLI(uint16_t addr);
void CLV(uint16_t addr);
void SEC(uint16_t addr);
void SED(uint16_t addr);
void SEI(uint16_t addr);

// Comparison instructions
void CMP(uint16_t addr);
void CPX(uint16_t addr);
void CPY(uint16_t addr);

// Conditional Branch instructions
void BCC(uint16_t addr);
void BCS(uint16_t addr);
void BEQ(uint16_t addr);
void BMI(uint16_t addr);
void BNE(uint16_t addr);
void BPL(uint16_t addr);
void BVC(uint16_t addr);
void BVS(uint16_t addr);

// Jumps & Subroutines
void JMP(uint16_t addr);
void JSR(uint16_t addr);
void RTS(uint16_t addr);

// Interrupts
void BRK(uint16_t addr);
void RTI(uint16_t addr);

// Others 
void BIT(uint16_t addr);
void NOP(uint16_t addr);
