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
        struct Instruction{
           uint8_t (*adressmode)(); 
           void (*code)(uint16_t input); 
        };
        Instruction table[256];
        uint8_t ImpliedMode();
        uint8_t ImmediateMode();
        uint8_t AbsoluteMode();

    public:

        mos6502();

        void Execute(Instruction i);
        void Run();

};
