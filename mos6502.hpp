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



    public:

        mos6502();

        void Execute(Instruction i);

};
