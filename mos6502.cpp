#include "mos6502.hpp"
#include "stdio.h"

#define TOGGLE_FLAG(expr,flag) expr ? status |= flag : status ^= flag
#define IS_SET(flag) (status & flag ? true : false)

mos6502::mos6502(ReadMem rm,WriteMem wm) : A(0),X(0),Y(0),status(0x0){

    this->Read = rm;
    this->Write = wm;

}

void mos6502::Push(byte m) {

    Write(m,0x0100 + this->sp);
    this->sp = sp == 0x00 ? 0xFF : sp - 1;

}

byte mos6502::Pop() {

    this->sp = sp == 0xFF ? 0x00 : sp + 1;

    return Read(0x0100 + this->sp);

}

void mos6502::LDA(uint16_t addr) {
    
    byte m = Read(addr);

    this->A = m;
    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(!this->A, flags::Z);

}

void mos6502::LDX(uint16_t addr) {
    
    byte m = Read(addr);

    this->X = m;
    TOGGLE_FLAG(this->X & 0x80, flags::N);
    TOGGLE_FLAG(!this->X, flags::Z);

}

void mos6502::LDY(uint16_t addr) {
    
    byte m = Read(addr);

    this->Y = m;
    TOGGLE_FLAG(this->Y & 0x80, flags::N);
    TOGGLE_FLAG(!this->Y, flags::Z);

}

void mos6502::STA(uint16_t addr) {
    Write(this->A,addr);
}

void mos6502::STX(uint16_t addr) {
    Write(this->X,addr);
}

void mos6502::STY(uint16_t addr) {
    Write(this->Y,addr);
}

void mos6502::TAX(uint16_t addr) {
    
    this->X = this->A;
    TOGGLE_FLAG(this->X & 0x80, flags::N);
    TOGGLE_FLAG(!this->X, flags::Z);

}

void mos6502::TAY(uint16_t addr) {
    

    this->Y = this->A;
    TOGGLE_FLAG(this->Y & 0x80, flags::N);
    TOGGLE_FLAG(!this->Y, flags::Z);

}

void mos6502::TSX(uint16_t addr) {
    

    this->X = sp;
    TOGGLE_FLAG(this->X & 0x80, flags::N);
    TOGGLE_FLAG(!this->X, flags::Z);

}

void mos6502::TXA(uint16_t addr) {
    

    this->A = this->X;
    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(!this->A, flags::Z);

}

void mos6502::TXS(uint16_t addr) {
    
    sp = this->X;

}

void mos6502::TYA(uint16_t addr) {
    

    this->A = this->Y;
    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(!this->A, flags::Z);

}

void mos6502::PHA(uint16_t addr) {
    
    this->Push(A);


}

void mos6502::PHP(uint16_t addr) {
    
    this->Push(this->status | flags::B | flags::R);

}

void mos6502::PLA(uint16_t addr) {
    
    this->A = this->Pop();
    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(!this->A, flags::Z);


}

void mos6502::PLP(uint16_t addr) {

    this->status = this->Pop() ^ flags::B ^ flags::R;

}

void mos6502::AND(uint16_t addr) {
    
    byte m = Read(addr);

    this->A &= m;
    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(!this->A, flags::Z);

}

void mos6502::EOR(uint16_t addr) {

    byte m = Read(addr);

    this->A ^= m;
    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(!this->A, flags::Z);

}

void mos6502::ORA(uint16_t addr) {

    byte m = Read(addr);
    this->A |= m;

    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(!this->A, flags::Z);

}

void mos6502::INC(uint16_t addr) {

    byte m = Read(addr);
    m++;
    Write(m,addr);

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(!m, flags::Z);

}

void mos6502::INY(uint16_t addr) {

    this->Y++;

    TOGGLE_FLAG(this->Y & 0x80, flags::N);
    TOGGLE_FLAG(!this->Y, flags::Z);

}

void mos6502::INX(uint16_t addr) {

    this->X++;

    TOGGLE_FLAG(this->X & 0x80, flags::N);
    TOGGLE_FLAG(!this->X, flags::Z);

}

void mos6502::DEC(uint16_t addr) {

    byte m = Read(addr);
    m--;
    Write(m,addr);

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(!m, flags::Z);

}

void mos6502::DEY(uint16_t addr) {

    this->Y--;

    TOGGLE_FLAG(this->Y & 0x80, flags::N);
    TOGGLE_FLAG(!this->Y, flags::Z);

}

void mos6502::DEX(uint16_t addr) {

    this->X--;

    TOGGLE_FLAG(this->X & 0x80, flags::N);
    TOGGLE_FLAG(!this->X, flags::Z);

}

void mos6502::ADC(uint16_t addr) {

    byte m = Read(addr);
    uint16_t res = this->A + m + IS_SET(flags::C);
    

    if(IS_SET(flags::D)) {

        byte low_nibble  = (this->A & 0x0F) + (m & 0x0F) + IS_SET(flags::C);     
        if(low_nibble > 9) res += 6;
        if(res > 0x99) res += 0x60;

        TOGGLE_FLAG(res > 0x99,flags::C);

    }
    else {

        TOGGLE_FLAG(res > 0xFF,flags::C);

    }

    // evaluates if the sign bit of the result is opposite of the operands
    TOGGLE_FLAG(((this->A >> 7) == (m >> 7) && (m >> 7) != (res >> 7)),flags::V);
    TOGGLE_FLAG(res & 0x80,flags::N);
    TOGGLE_FLAG(!res, flags::Z);
    this->A = res & 0xFF;

}

void mos6502::SBC(uint16_t addr) {


    byte m = Read(addr);
    uint16_t res = this->A - m - IS_SET(flags::C);
    

    if(IS_SET(flags::D)) {

        if(((this->A & 0x0F) - IS_SET(flags::C)) < (m & 0x0F)) res -= 6;
        if(res > 0x99) res -= 0x60;

    }

    TOGGLE_FLAG(res < 0x100,flags::C);
    // evaluates if the sign bit of the result is opposite of the operands
    TOGGLE_FLAG(((this->A >> 7) == (m >> 7) && (m >> 7) != (res >> 7)),flags::V);
    TOGGLE_FLAG(res & 0x80,flags::N);
    TOGGLE_FLAG(!res, flags::Z);
    this->A = res & 0xFF;
}

void mos6502::ASL(uint16_t addr) {

    byte m = Read(addr);
    TOGGLE_FLAG(m & 0x80,flags::C);
    m = m << 1;

    Write(m,addr);

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(!m, flags::Z);

}

void mos6502::LSR(uint16_t addr) {

    byte m = Read(addr);
    TOGGLE_FLAG(m & 0x01,flags::C);
    m = m >> 1;

    Write(m,addr);

    TOGGLE_FLAG(0,flags::N);
    TOGGLE_FLAG(!m, flags::Z);

}

void mos6502::ROL(uint16_t addr) {

    uint16_t m = Read(addr);

    if(IS_SET(flags::C)) {
        m |= 0x01;
    }

    m = m << 1;
    TOGGLE_FLAG(m > 0xFF,flags::C);

    m &= 0xFF;
    Write(m,addr);

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(!m, flags::Z);

}

void mos6502::ROR(uint16_t addr) {

    uint16_t m = Read(addr);

    if(IS_SET(flags::C)) {
        m |= 0x100;
    }

    m = m >> 1;
    TOGGLE_FLAG(m & 0x01,flags::C);

    m &= 0xFF;
    Write(m,addr);

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(!m, flags::Z);

}

void mos6502::ASL_A(uint16_t addr) {

    byte m = this->A;
    TOGGLE_FLAG(m & 0x80,flags::C);
    m = m << 1;

    this->A = m;

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(!m, flags::Z);

}

void mos6502::LSR_A(uint16_t addr) {

    byte m = this->A;
    TOGGLE_FLAG(m & 0x01,flags::C);
    m = m >> 1;

    this->A = m;

    TOGGLE_FLAG(0,flags::N);
    TOGGLE_FLAG(!m, flags::Z);

}

void mos6502::ROL_A(uint16_t addr) {

    uint16_t m = this->A;

    if(IS_SET(flags::C)) {
        m |= 0x01;
    }

    m = m << 1;
    TOGGLE_FLAG(m > 0xFF,flags::C);

    m &= 0xFF;
    this->A = m;

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(!m, flags::Z);

}

void mos6502::ROR_A(uint16_t addr) {

    uint16_t m = this->A;

    if(IS_SET(flags::C)) {
        m |= 0x100;
    }

    m = m >> 1;
    TOGGLE_FLAG(m & 0x01,flags::C);

    m &= 0xFF;
    this->A = m;

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(!m, flags::Z);

}

void mos6502::CLC(uint16_t addr) {

    TOGGLE_FLAG(0,flags::C);

}

void mos6502::CLD(uint16_t addr) {

    TOGGLE_FLAG(0,flags::D);

}

void mos6502::CLI(uint16_t addr) {

    TOGGLE_FLAG(0,flags::I);

}

void mos6502::CLV(uint16_t addr) {

    TOGGLE_FLAG(0,flags::V);

}

void mos6502::SEC(uint16_t addr) {

    TOGGLE_FLAG(1,flags::C);

}

void mos6502::SED(uint16_t addr) {

    TOGGLE_FLAG(1,flags::D);

}

void mos6502::SEI(uint16_t addr) {

    TOGGLE_FLAG(1,flags::I);

}

void mos6502::CMP(uint16_t addr) {
    
    byte m = Read(addr);
    uint16_t res = this->A - m; 
    TOGGLE_FLAG(res <= 0xFF,flags::C);
    TOGGLE_FLAG(res & 0x80,flags::N);
    TOGGLE_FLAG(!(res & 0xFF),flags::Z);

}


void mos6502::CPX(uint16_t addr) {

    byte m = Read(addr);
    uint16_t res = this->X - m; 
    TOGGLE_FLAG(res <= 0xFF,flags::C);
    TOGGLE_FLAG(res & 0x80,flags::N);
    TOGGLE_FLAG(!(res & 0xFF),flags::Z);

}

void mos6502::CPY(uint16_t addr) {

    byte m = Read(addr);
    uint16_t res = this->Y - m; 
    TOGGLE_FLAG(res <= 0xFF,flags::C);
    TOGGLE_FLAG(res & 0x80,flags::N);
    TOGGLE_FLAG(!(res & 0xFF),flags::Z);

}

