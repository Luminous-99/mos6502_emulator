#include "mos6502.hpp"
#include "stdio.h"

#define TOGGLE_FLAG(expr,flag) expr ? status |= flag : status &= ~flag

mos6502::mos6502(ReadMem rm,WriteMem wm) : A(0),X(0),Y(0),status(0x0){

    this->Read = rm;
    this->Write = wm;

}

void mos6502::AND(uint16_t addr) {
    
    uint8_t m = Read(addr);

    this->A &= m;
    TOGGLE_FLAG(this->A & flags::N, flags::N);
    TOGGLE_FLAG(this->A, flags::Z);

}

void mos6502::EOR(uint16_t addr) {

    uint8_t m = Read(addr);

    this->A ^= m;
    TOGGLE_FLAG(this->A & flags::N, flags::N);
    TOGGLE_FLAG(this->A, flags::Z);

}

void mos6502::ORA(uint16_t addr) {

    uint8_t m = Read(addr);
    this->A |= m;

    TOGGLE_FLAG(this->A & flags::N, flags::N);
    TOGGLE_FLAG(this->A, flags::Z);

}

void mos6502::INC(uint16_t addr) {

    uint8_t m = Read(addr);
    m++;
    Write(m,addr);

    TOGGLE_FLAG(m & flags::N, flags::N);
    TOGGLE_FLAG(m, flags::Z);

}

void mos6502::INY(uint16_t addr) {

    this->Y++;

    TOGGLE_FLAG(this->Y & flags::N, flags::N);
    TOGGLE_FLAG(this->Y, flags::Z);

}

void mos6502::INX(uint16_t addr) {

    this->X++;

    TOGGLE_FLAG(this->X & flags::N, flags::N);
    TOGGLE_FLAG(this->X, flags::Z);

}

void mos6502::DEC(uint16_t addr) {

    uint8_t m = Read(addr);
    m--;
    Write(m,addr);

    TOGGLE_FLAG(m & flags::N, flags::N);
    TOGGLE_FLAG(m, flags::Z);

}

void mos6502::DEY(uint16_t addr) {

    this->Y--;

    TOGGLE_FLAG(this->Y & flags::N, flags::N);
    TOGGLE_FLAG(this->Y, flags::Z);

}

void mos6502::DEX(uint16_t addr) {

    this->X--;

    TOGGLE_FLAG(this->X & flags::N, flags::N);
    TOGGLE_FLAG(this->X, flags::Z);

}
