#pragma once
#include <stdint.h>

typedef uint8_t byte;

enum flags{
    C = 0b00000001,       // Carry flag 
    Z = 0b00000010,      // Zero flag 
    I = 0b00000100,     // Interrupt Disable flag 
    D = 0b00001000,    // Decimal flag 
    B = 0b00010000,   // Break flag  
    R = 0b00100000,  // Reserved flag
    V = 0b01000000, // Overflow flag 
    N = 0b10000000 // Negative flag 
};

class mos6502{
    private:
        byte A; // Accumulator
        byte X; // index Register X
        byte Y; // index Register Y
        byte status; // Status flags
        byte sp; // Stack pointer
        uint16_t pc; // Program counter
                     
        typedef void (*Code)(byte addr);
        typedef uint16_t (*Addr)();
        typedef byte (*ReadMem)(uint16_t addr);
        typedef void (*WriteMem)(byte m,uint16_t addr);

        ReadMem Read;
        WriteMem Write;


        byte Pop();
        void Push(byte m);

        struct Instruction{
           Addr mode; 
           Code code; 
        };

        Instruction table[256];


        // Adress modes
        uint16_t Accumulator();
        uint16_t AbsoluteMode();
        uint16_t AbsoluteXMode();
        uint16_t AbsoluteYMode();
        uint16_t ImmediateMode();
        uint16_t ImpliedMode();
        uint16_t IndirectMode();
        uint16_t IndirectXMode();
        uint16_t IndirectYMode();
        uint16_t RelativeMode();
        uint16_t ZeroPageMode();
        uint16_t ZeroPageXMode();
        uint16_t ZeroPageYMode();

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

        // Instructions
        void BRK(uint16_t addr);
        void BPL(uint16_t addr);
        void JSR(uint16_t addr);
        void BIT(uint16_t addr);
        void BMI(uint16_t addr);
        void RTI(uint16_t addr);
        void JMP(uint16_t addr);
        void BVC(uint16_t addr);
        void RTS(uint16_t addr);
        void ADC(uint16_t addr);
        void BVS(uint16_t addr);
        void BCC(uint16_t addr);
        void BCS(uint16_t addr);
        void CPY(uint16_t addr);
        void CMP(uint16_t addr);
        void BNE(uint16_t addr);
        void CPX(uint16_t addr);
        void SBC(uint16_t addr);
        void NOP(uint16_t addr);
        void BEQ(uint16_t addr);



    public:

        mos6502(mos6502::ReadMem rm,mos6502::WriteMem wm);

        void Execute(Instruction i);

};
