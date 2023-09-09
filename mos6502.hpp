#pragma once
#include <stdint.h>

enum class flags{
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
                     //
        struct Instruction{
           uint16_t (*adressmode)(); 
           void (*code)(uint16_t input); 
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
        void BRK(uint16_t input);
        void ORA(uint16_t input);
        void ASL(uint16_t input);
        void PHP(uint16_t input);
        void BPL(uint16_t input);
        void CLC(uint16_t input);
        void CLD(uint16_t input);
        void JSR(uint16_t input);
        void AND(uint16_t input);
        void BIT(uint16_t input);
        void ROL(uint16_t input);
        void PLP(uint16_t input);
        void BMI(uint16_t input);
        void SEC(uint16_t input);
        void RTI(uint16_t input);
        void EOR(uint16_t input);
        void LSR(uint16_t input);
        void PHA(uint16_t input);
        void JMP(uint16_t input);
        void BVC(uint16_t input);
        void CLI(uint16_t input);
        void RTS(uint16_t input);
        void ADC(uint16_t input);
        void ROR(uint16_t input);
        void PLA(uint16_t input);
        void BVS(uint16_t input);
        void SEI(uint16_t input);
        void STA(uint16_t input);
        void STY(uint16_t input);
        void STX(uint16_t input);
        void DEY(uint16_t input);
        void DEX(uint16_t input);
        void TXA(uint16_t input);
        void BCC(uint16_t input);
        void TYA(uint16_t input);
        void TXS(uint16_t input);
        void LDY(uint16_t input);
        void LDA(uint16_t input);
        void LDX(uint16_t input);
        void TAY(uint16_t input);
        void TAX(uint16_t input);
        void BCS(uint16_t input);
        void CLV(uint16_t input);
        void TSX(uint16_t input);
        void CPY(uint16_t input);
        void CMP(uint16_t input);
        void DEC(uint16_t input);
        void INY(uint16_t input);
        void BNE(uint16_t input);
        void CPX(uint16_t input);
        void SBC(uint16_t input);
        void INC(uint16_t input);
        void INX(uint16_t input);
        void NOP(uint16_t input);
        void BEQ(uint16_t input);
        void SED(uint16_t input);



    public:

        mos6502();

        void Execute(Instruction i);

};
