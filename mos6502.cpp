#include "mos6502.hpp"
#include "stdio.h"

#define TOGGLE_FLAG(expr,flag) expr ? status |= flag : status &= ~flag

mos6502::mos6502() : A(0),X(0),Y(0),status(0x0){

}

void mos6502::AND(uint8_t val) {
    
    this->A &= val;
    TOGGLE_FLAG(this->A & flags::N, flags::N);
    TOGGLE_FLAG(this->A, flags::Z);

}

void mos6502::EOR(uint8_t val) {

    this->A ^= val;
    TOGGLE_FLAG(this->A & flags::N, flags::N);
    TOGGLE_FLAG(this->A, flags::Z);

}

void mos6502::ORA(uint8_t val) {

    this->A |= val;
    TOGGLE_FLAG(this->A & flags::N, flags::N);
    TOGGLE_FLAG(this->A, flags::Z);

}
