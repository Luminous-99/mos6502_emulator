#pragma once
#include <stdint.h>


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
        uint8_t A; // Accumulator
        uint8_t X; // index Register X
        uint8_t Y; // index Register Y
        uint8_t status; // Status flags
        uint8_t* sp; // Stack pointer
        uint16_t pc; // Program counter
                     
        typedef void (mos6502::*Code)(uint8_t val);
        typedef uint16_t (mos6502::*Addr)();

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

        // Instuctions
        void BRK(uint8_t val);
        void ORA(uint8_t val);
        void ASL(uint8_t val);
        void PHP(uint8_t val);
        void BPL(uint8_t val);
        void CLC(uint8_t val);
        void CLD(uint8_t val);
        void JSR(uint8_t val);
        void AND(uint8_t val);
        void BIT(uint8_t val);
        void ROL(uint8_t val);
        void PLP(uint8_t val);
        void BMI(uint8_t val);
        void SEC(uint8_t val);
        void RTI(uint8_t val);
        void EOR(uint8_t val);
        void LSR(uint8_t val);
        void PHA(uint8_t val);
        void JMP(uint8_t val);
        void BVC(uint8_t val);
        void CLI(uint8_t val);
        void RTS(uint8_t val);
        void ADC(uint8_t val);
        void ROR(uint8_t val);
        void PLA(uint8_t val);
        void BVS(uint8_t val);
        void SEI(uint8_t val);
        void STA(uint8_t val);
        void STY(uint8_t val);
        void STX(uint8_t val);
        void DEY(uint8_t val);
        void DEX(uint8_t val);
        void TXA(uint8_t val);
        void BCC(uint8_t val);
        void TYA(uint8_t val);
        void TXS(uint8_t val);
        void LDY(uint8_t val);
        void LDA(uint8_t val);
        void LDX(uint8_t val);
        void TAY(uint8_t val);
        void TAX(uint8_t val);
        void BCS(uint8_t val);
        void CLV(uint8_t val);
        void TSX(uint8_t val);
        void CPY(uint8_t val);
        void CMP(uint8_t val);
        void DEC(uint8_t val);
        void INY(uint8_t val);
        void BNE(uint8_t val);
        void CPX(uint8_t val);
        void SBC(uint8_t val);
        void INC(uint8_t val);
        void INX(uint8_t val);
        void NOP(uint8_t val);
        void BEQ(uint8_t val);
        void SED(uint8_t val);



    public:

        mos6502();

        void Execute(Instruction i);

};
