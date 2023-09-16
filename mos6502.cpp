#include "mos6502.hpp"
#include "stdio.h"

#define TOGGLE_FLAG(expr,flag) expr ? status |= flag : status &= ~flag
#define IF_SET(flag) status & flag

mos6502::mos6502(ReadMem rm,WriteMem wm) : A(0),X(0),Y(0),status(0x0){

    this->Read = rm;
    this->Write = wm;

}

void mos6502::AND(uint16_t addr) {
    
    uint8_t m = Read(addr);

    this->A &= m;
    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(this->A, flags::Z);

}

void mos6502::EOR(uint16_t addr) {

    uint8_t m = Read(addr);

    this->A ^= m;
    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(this->A, flags::Z);

}

void mos6502::ORA(uint16_t addr) {

    uint8_t m = Read(addr);
    this->A |= m;

    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(this->A, flags::Z);

}

void mos6502::INC(uint16_t addr) {

    uint8_t m = Read(addr);
    m++;
    Write(m,addr);

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(m, flags::Z);

}

void mos6502::INY() {

    this->Y++;

    TOGGLE_FLAG(this->Y & 0x80, flags::N);
    TOGGLE_FLAG(this->Y, flags::Z);

}

void mos6502::INX() {

    this->X++;

    TOGGLE_FLAG(this->X & 0x80, flags::N);
    TOGGLE_FLAG(this->X, flags::Z);

}

void mos6502::DEC(uint16_t addr) {

    uint8_t m = Read(addr);
    m--;
    Write(m,addr);

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(m, flags::Z);

}

void mos6502::DEY() {

    this->Y--;

    TOGGLE_FLAG(this->Y & 0x80, flags::N);
    TOGGLE_FLAG(this->Y, flags::Z);

}

void mos6502::DEX() {

    this->X--;

    TOGGLE_FLAG(this->X & 0x80, flags::N);
    TOGGLE_FLAG(this->X, flags::Z);

}

void mos6502::ASL(uint16_t addr) {

    uint8_t m = Read(addr);
    TOGGLE_FLAG(m & 0x80,flags::C);
    m = m << 1;

    Write(m,addr);

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(m, flags::Z);

}

void mos6502::LSR(uint16_t addr) {

    uint8_t m = Read(addr);
    TOGGLE_FLAG(m & 0x01,flags::C);
    m = m >> 1;

    Write(m,addr);

    TOGGLE_FLAG(0,flags::N);
    TOGGLE_FLAG(m, flags::Z);

}

void mos6502::ROL(uint16_t addr) {

    uint16_t m = Read(addr);

    if(IF_SET(flags::C)) {
        m |= 0x01;
    }

    m = m << 1;
    TOGGLE_FLAG(m > 0xFF,flags::C);

    m &= 0xFF;
    Write(m,addr);

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(m, flags::Z);

}

void mos6502::ROR(uint16_t addr) {

    uint16_t m = Read(addr);

    if(IF_SET(flags::C)) {
        m |= 0x100;
    }

    m = m >> 1;
    TOGGLE_FLAG(m & 0x01,flags::C);

    m &= 0xFF;
    Write(m,addr);

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(m, flags::Z);

}
