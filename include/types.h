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

typedef void     (*Code)(uint16_t addr);
typedef uint16_t (*Addr)();

typedef byte (*ReadMem)(uint16_t addr);
typedef void (*WriteMem)(byte m,uint16_t addr);

typedef struct {

   Addr mode; 
   Code code; 

} Instruction;

