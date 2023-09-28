#include "mos6502.hpp"

#define TOGGLE_FLAG(expr,flag) expr ? status |= flag : status ^= flag
#define IS_SET(flag) (status & flag ? true : false)

mos6502::mos6502(ReadMem rm,WriteMem wm) : A(0),X(0),Y(0),status(0x0){

    this->Read = rm;
    this->Write = wm;

    this->table[0x69] = { &mos6502::ImmediateMode , &mos6502::ADC };
    this->table[0x65] = { &mos6502::ZeroPageMode ,  &mos6502::ADC };
    this->table[0x75] = { &mos6502::ZeroPageXMode , &mos6502::ADC };
    this->table[0x6D] = { &mos6502::AbsoluteMode ,  &mos6502::ADC };
    this->table[0x7D] = { &mos6502::AbsoluteXMode , &mos6502::ADC };
    this->table[0x79] = { &mos6502::AbsoluteYMode , &mos6502::ADC };
    this->table[0x61] = { &mos6502::IndirectXMode , &mos6502::ADC };
    this->table[0x71] = { &mos6502::IndirectYMode , &mos6502::ADC };

    this->table[0x29] = { &mos6502::ImmediateMode , &mos6502::AND };
    this->table[0x25] = { &mos6502::ZeroPageMode ,  &mos6502::AND };
    this->table[0x35] = { &mos6502::ZeroPageXMode , &mos6502::AND };
    this->table[0x2D] = { &mos6502::AbsoluteMode ,  &mos6502::AND };
    this->table[0x3D] = { &mos6502::AbsoluteXMode , &mos6502::AND };
    this->table[0x39] = { &mos6502::AbsoluteYMode , &mos6502::AND };
    this->table[0x21] = { &mos6502::IndirectXMode , &mos6502::AND };
    this->table[0x31] = { &mos6502::IndirectYMode , &mos6502::AND };

    this->table[0x0A] = { &mos6502::AccumulatorMode,&mos6502::ASL_A };
    this->table[0x06] = { &mos6502::ZeroPageMode ,  &mos6502::ASL };
    this->table[0x16] = { &mos6502::ZeroPageXMode , &mos6502::ASL };
    this->table[0x0E] = { &mos6502::AbsoluteMode ,  &mos6502::ASL };
    this->table[0x1E] = { &mos6502::AbsoluteXMode , &mos6502::ASL };

    this->table[0x24] = { &mos6502::ZeroPageMode ,  &mos6502::BIT };
    this->table[0x2C] = { &mos6502::AbsoluteMode ,  &mos6502::BIT };

    this->table[0x00] = { &mos6502::ImpliedMode ,   &mos6502::BRK };

    this->table[0x18] = { &mos6502::ImpliedMode ,   &mos6502::CLC };
    this->table[0xD8] = { &mos6502::ImpliedMode ,   &mos6502::CLD };
    this->table[0x58] = { &mos6502::ImpliedMode ,   &mos6502::CLI };
    this->table[0xB8] = { &mos6502::ImpliedMode ,   &mos6502::CLV };

    this->table[0xC9] = { &mos6502::ImmediateMode , &mos6502::CMP };
    this->table[0xC5] = { &mos6502::ZeroPageMode ,  &mos6502::CMP };
    this->table[0xD5] = { &mos6502::ZeroPageXMode , &mos6502::CMP };
    this->table[0xCD] = { &mos6502::AbsoluteMode ,  &mos6502::CMP };
    this->table[0xDD] = { &mos6502::AbsoluteXMode , &mos6502::CMP };
    this->table[0xD9] = { &mos6502::AbsoluteYMode , &mos6502::CMP };
    this->table[0xC1] = { &mos6502::IndirectXMode , &mos6502::CMP };
    this->table[0xD1] = { &mos6502::IndirectYMode , &mos6502::CMP };
    
    this->table[0xE0] = { &mos6502::ImmediateMode , &mos6502::CPX };
    this->table[0xE4] = { &mos6502::ZeroPageMode ,  &mos6502::CPX };
    this->table[0xEC] = { &mos6502::AbsoluteMode ,  &mos6502::CPX };

    this->table[0xC0] = { &mos6502::ImmediateMode , &mos6502::CPY };
    this->table[0xC4] = { &mos6502::ZeroPageMode  , &mos6502::CPY };
    this->table[0xCC] = { &mos6502::AbsoluteMode  , &mos6502::CPY };

    this->table[0xC6] = { &mos6502::ZeroPageMode ,  &mos6502::DEC };
    this->table[0xD6] = { &mos6502::ZeroPageXMode , &mos6502::DEC };
    this->table[0xCE] = { &mos6502::AbsoluteMode ,  &mos6502::DEC };
    this->table[0xDE] = { &mos6502::AbsoluteXMode , &mos6502::DEC };

    this->table[0xE6] = { &mos6502::ZeroPageMode ,  &mos6502::INC };
    this->table[0xF6] = { &mos6502::ZeroPageXMode , &mos6502::INC };
    this->table[0xEE] = { &mos6502::AbsoluteMode ,  &mos6502::INC };
    this->table[0xFE] = { &mos6502::AbsoluteXMode , &mos6502::INC };

    this->table[0xCA] = { &mos6502::ImpliedMode ,   &mos6502::DEX };
    this->table[0x88] = { &mos6502::ImpliedMode ,   &mos6502::DEY };
    this->table[0xE8] = { &mos6502::ImpliedMode ,   &mos6502::INX };
    this->table[0xC8] = { &mos6502::ImpliedMode ,   &mos6502::INY };

    this->table[0x49] = { &mos6502::ImmediateMode , &mos6502::EOR };
    this->table[0x45] = { &mos6502::ZeroPageMode ,  &mos6502::EOR };
    this->table[0x55] = { &mos6502::ZeroPageXMode , &mos6502::EOR };
    this->table[0x4D] = { &mos6502::AbsoluteMode ,  &mos6502::EOR };
    this->table[0x5D] = { &mos6502::AbsoluteXMode , &mos6502::EOR };
    this->table[0x59] = { &mos6502::AbsoluteYMode , &mos6502::EOR };
    this->table[0x41] = { &mos6502::IndirectXMode , &mos6502::EOR };
    this->table[0x51] = { &mos6502::IndirectYMode , &mos6502::EOR };

    this->table[0x4C] = { &mos6502::AbsoluteMode ,  &mos6502::JMP };
    this->table[0x6C] = { &mos6502::IndirectMode ,  &mos6502::JMP };
    
    this->table[0x20] = { &mos6502::AbsoluteMode ,  &mos6502::JSR };

    this->table[0xA9] = { &mos6502::ImmediateMode , &mos6502::LDA };
    this->table[0xA5] = { &mos6502::ZeroPageMode ,  &mos6502::LDA };
    this->table[0xB5] = { &mos6502::ZeroPageXMode , &mos6502::LDA };
    this->table[0xAD] = { &mos6502::AbsoluteMode ,  &mos6502::LDA };
    this->table[0xBD] = { &mos6502::AbsoluteXMode , &mos6502::LDA };
    this->table[0xB9] = { &mos6502::AbsoluteYMode , &mos6502::LDA };
    this->table[0xA1] = { &mos6502::IndirectXMode , &mos6502::LDA };
    this->table[0xB1] = { &mos6502::IndirectYMode , &mos6502::LDA };


    this->table[0xA2] = { &mos6502::ImmediateMode , &mos6502::LDX };
    this->table[0xA6] = { &mos6502::ZeroPageMode ,  &mos6502::LDX };
    this->table[0xB6] = { &mos6502::ZeroPageYMode , &mos6502::LDX };
    this->table[0xAE] = { &mos6502::AbsoluteMode ,  &mos6502::LDX };
    this->table[0xBE] = { &mos6502::AbsoluteYMode , &mos6502::LDX };

    this->table[0xA0] = { &mos6502::ImmediateMode , &mos6502::LDY };
    this->table[0xA4] = { &mos6502::ZeroPageMode ,  &mos6502::LDY };
    this->table[0xB4] = { &mos6502::ZeroPageXMode , &mos6502::LDY };
    this->table[0xAC] = { &mos6502::AbsoluteMode ,  &mos6502::LDY };
    this->table[0xBC] = { &mos6502::AbsoluteXMode , &mos6502::LDY };

    this->table[0x4A] = { &mos6502::AccumulatorMode,&mos6502::LSR_A };
    this->table[0x46] = { &mos6502::ZeroPageMode ,  &mos6502::LSR };
    this->table[0x56] = { &mos6502::ZeroPageXMode , &mos6502::LSR };
    this->table[0x4E] = { &mos6502::AbsoluteMode ,  &mos6502::LSR };
    this->table[0x5E] = { &mos6502::AbsoluteXMode , &mos6502::LSR };

    this->table[0xEA] = { &mos6502::ImpliedMode ,   &mos6502::NOP };

    this->table[0x09] = { &mos6502::ImmediateMode , &mos6502::ORA };
    this->table[0x05] = { &mos6502::ZeroPageMode ,  &mos6502::ORA };
    this->table[0x15] = { &mos6502::ZeroPageXMode , &mos6502::ORA };
    this->table[0x0D] = { &mos6502::AbsoluteMode ,  &mos6502::ORA };
    this->table[0x1D] = { &mos6502::AbsoluteXMode , &mos6502::ORA };
    this->table[0x19] = { &mos6502::AbsoluteYMode , &mos6502::ORA };
    this->table[0x01] = { &mos6502::IndirectXMode , &mos6502::ORA };
    this->table[0x11] = { &mos6502::IndirectYMode , &mos6502::ORA };

    this->table[0x48] = { &mos6502::ImpliedMode ,   &mos6502::PHA };
    this->table[0x08] = { &mos6502::ImpliedMode ,   &mos6502::PHP };
    this->table[0x68] = { &mos6502::ImpliedMode ,   &mos6502::PLA };
    this->table[0x28] = { &mos6502::ImpliedMode ,   &mos6502::PLA };

    this->table[0x2A] = { &mos6502::AccumulatorMode,&mos6502::ROL_A };
    this->table[0x26] = { &mos6502::ZeroPageMode ,  &mos6502::ROL };
    this->table[0x36] = { &mos6502::ZeroPageXMode , &mos6502::ROL };
    this->table[0x2E] = { &mos6502::AbsoluteMode ,  &mos6502::ROL };
    this->table[0x3E] = { &mos6502::AbsoluteXMode , &mos6502::ROL };

    this->table[0x6A] = { &mos6502::AccumulatorMode,&mos6502::ROR_A };
    this->table[0x66] = { &mos6502::ZeroPageMode ,  &mos6502::ROR };
    this->table[0x76] = { &mos6502::ZeroPageXMode , &mos6502::ROR };
    this->table[0x6E] = { &mos6502::AbsoluteMode ,  &mos6502::ROR };
    this->table[0x7E] = { &mos6502::AbsoluteXMode , &mos6502::ROR };

    this->table[0x40] = { &mos6502::ImpliedMode ,   &mos6502::RTI };

    this->table[0x60] = { &mos6502::ImpliedMode ,   &mos6502::RTS };

    this->table[0xE9] = { &mos6502::ImmediateMode , &mos6502::SBC };
    this->table[0xE5] = { &mos6502::ZeroPageMode ,  &mos6502::SBC };
    this->table[0xF5] = { &mos6502::ZeroPageXMode , &mos6502::SBC };
    this->table[0xED] = { &mos6502::AbsoluteMode ,  &mos6502::SBC };
    this->table[0xFD] = { &mos6502::AbsoluteXMode , &mos6502::SBC };
    this->table[0xF9] = { &mos6502::AbsoluteYMode , &mos6502::SBC };
    this->table[0xE1] = { &mos6502::IndirectXMode , &mos6502::SBC };
    this->table[0xF1] = { &mos6502::IndirectYMode , &mos6502::SBC };

    this->table[0x38] = { &mos6502::ImpliedMode ,   &mos6502::SEC };
    this->table[0xF8] = { &mos6502::ImpliedMode ,   &mos6502::SED };
    this->table[0x78] = { &mos6502::ImpliedMode ,   &mos6502::SEI };

    this->table[0x85] = { &mos6502::ZeroPageMode ,  &mos6502::STA };
    this->table[0x95] = { &mos6502::ZeroPageXMode , &mos6502::STA };
    this->table[0x8D] = { &mos6502::AbsoluteMode ,  &mos6502::STA };
    this->table[0x9D] = { &mos6502::AbsoluteXMode , &mos6502::STA };
    this->table[0x99] = { &mos6502::AbsoluteYMode , &mos6502::STA };
    this->table[0x81] = { &mos6502::IndirectXMode , &mos6502::STA };
    this->table[0x91] = { &mos6502::IndirectYMode , &mos6502::STA };

    this->table[0x86] = { &mos6502::ZeroPageMode ,  &mos6502::STX };
    this->table[0x96] = { &mos6502::ZeroPageYMode , &mos6502::STX };
    this->table[0x8E] = { &mos6502::AbsoluteMode ,  &mos6502::STX };

    this->table[0x84] = { &mos6502::ZeroPageMode ,  &mos6502::STY };
    this->table[0x94] = { &mos6502::ZeroPageXMode , &mos6502::STY };
    this->table[0x8C] = { &mos6502::AbsoluteMode ,  &mos6502::STY };

    this->table[0xAA] = { &mos6502::ImpliedMode ,   &mos6502::TAX };
    this->table[0xA8] = { &mos6502::ImpliedMode ,   &mos6502::TAY };
    this->table[0xBA] = { &mos6502::ImpliedMode ,   &mos6502::TSX };
    this->table[0x8A] = { &mos6502::ImpliedMode ,   &mos6502::TXA };
    this->table[0x9A] = { &mos6502::ImpliedMode ,   &mos6502::TXS };
    this->table[0x98] = { &mos6502::ImpliedMode ,   &mos6502::TYA };

    this->table[0x90] = { &mos6502::RelativeMode ,  &mos6502::BCC };
    this->table[0xB0] = { &mos6502::RelativeMode ,  &mos6502::BCS };
    this->table[0xF0] = { &mos6502::RelativeMode ,  &mos6502::BEQ };
    this->table[0x30] = { &mos6502::RelativeMode ,  &mos6502::BMI };
    this->table[0xD0] = { &mos6502::RelativeMode ,  &mos6502::BNE };
    this->table[0x10] = { &mos6502::RelativeMode ,  &mos6502::BPL };
    this->table[0x50] = { &mos6502::RelativeMode ,  &mos6502::BVC };
    this->table[0x70] = { &mos6502::RelativeMode ,  &mos6502::BVS };

}

void mos6502::Execute(mos6502::Instruction i) {
    uint16_t addr = (this->*i.mode)();
    (this->*i.code)(addr);
}

void mos6502::Run() {

    Instruction ins;
    byte opcode;

    while(1) {

        opcode = this->Read(pc++);

        ins = table[opcode];

        this->Execute(ins);

    }

}

void mos6502::NMI() {
    
    this->Push(this->pc >> 8);
    this->Push(this->pc & 0xFF);
    this->Push(this->status);

    TOGGLE_FLAG(1,flags::I);

    uint16_t low_nibble =  this->Read(0xFFFA);
    uint16_t high_nibble = (this->Read(0xFFFB) << 8);

    this->pc = high_nibble | low_nibble;

}

void mos6502::IRQ() {
        
    if(!(IS_SET(flags::I))) {

        this->Push(this->pc >> 8);
        this->Push(this->pc & 0xFF);
        this->Push(this->status);

        TOGGLE_FLAG(1,flags::I);

        uint16_t low_nibble =  this->Read(0xFFFE);
        uint16_t high_nibble = (this->Read(0xFFFF) << 8);

        this->pc = high_nibble | low_nibble;
    }

}

void mos6502::Reset() {

    TOGGLE_FLAG(1, flags::I);
    
    uint16_t low_nibble  = this->Read(0xFFFC);
    uint16_t high_nibble = (this->Read(0xFFFD) << 8);

    this->pc = high_nibble | low_nibble;

}


uint16_t mos6502::ImpliedMode() {

    return pc++;

}

uint16_t mos6502::AccumulatorMode() {

    return pc++;

}

uint16_t mos6502::AbsoluteMode() {

    uint16_t low_nibble = this->Read(pc++);
    uint16_t high_nibble = this->Read(pc++);

    return low_nibble | (high_nibble << 8);

}

uint16_t mos6502::AbsoluteXMode() {

    uint16_t low_nibble = this->Read(pc++);
    uint16_t high_nibble = this->Read(pc++);

    return (low_nibble | (high_nibble << 8)) + this->X + IS_SET(flags::C);

}

uint16_t mos6502::AbsoluteYMode() {

    uint16_t low_nibble = this->Read(pc++);
    uint16_t high_nibble = this->Read(pc++);

    return (low_nibble | (high_nibble << 8)) + this->Y + IS_SET(flags::C);

}

uint16_t mos6502::ImmediateMode() {

    return pc++;

}

uint16_t mos6502::ZeroPageMode() {

    return this->Read(pc++);

}

uint16_t mos6502::ZeroPageXMode() {

    return (this->Read(pc++) + this->X) & 0xFF;

}

uint16_t mos6502::ZeroPageYMode() {

    return (this->Read(pc++) + this->Y) & 0xFF;

}

uint16_t mos6502::IndirectMode() {

    uint16_t low_nibble = this->Read(pc++);
    uint16_t high_nibble = this->Read(pc++);
    uint16_t addr = (low_nibble) | (high_nibble << 8);
    uint16_t eff = this->Read(addr) | (this->Read((addr + 1) & 0xFF) << 8);

    return eff;

}

uint16_t mos6502::IndirectXMode() {

    uint16_t zlow_nib = (this->Read(pc++) + this->X) & 0xFF;
    uint16_t zhigh_nib = (zlow_nib + 1) & 0xFF;
    uint16_t addr = this->Read(zlow_nib) | (this->Read(zhigh_nib) << 8);

    return addr;
}

uint16_t mos6502::IndirectYMode() {

    uint16_t zlow_nib = (this->Read(pc++)) & 0xFF;
    uint16_t zhigh_nib = (zlow_nib + 1) & 0xFF;
    uint16_t addr = (this->Read(zlow_nib) | (this->Read(zhigh_nib) << 8)) + this->Y;

    return addr;
}

uint16_t mos6502::RelativeMode() {

    uint16_t offset = this->Read(pc++);
    offset |= offset & 0x80 ? 0xFF00 : 0;
    
    return this->pc + offset;

}

void mos6502::Push(byte m) {

    Write(m,0x0100 + this->sp);
    this->sp = sp == 0x00 ? 0xFF : sp - 1;

}

byte mos6502::Pop() {

    this->sp = sp == 0xFF ? 0x00 : sp + 1;

    return this->Read(0x0100 + this->sp);

}

void mos6502::LDA(uint16_t addr) {
    
    byte m = this->Read(addr);

    this->A = m;
    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(!this->A, flags::Z);

}

void mos6502::LDX(uint16_t addr) {
    
    byte m = this->Read(addr);

    this->X = m;
    TOGGLE_FLAG(this->X & 0x80, flags::N);
    TOGGLE_FLAG(!this->X, flags::Z);

}

void mos6502::LDY(uint16_t addr) {
    
    byte m = this->Read(addr);

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
    
    byte m = this->Read(addr);

    this->A &= m;
    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(!this->A, flags::Z);

}

void mos6502::EOR(uint16_t addr) {

    byte m = this->Read(addr);

    this->A ^= m;
    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(!this->A, flags::Z);

}

void mos6502::ORA(uint16_t addr) {

    byte m = this->Read(addr);
    this->A |= m;

    TOGGLE_FLAG(this->A & 0x80, flags::N);
    TOGGLE_FLAG(!this->A, flags::Z);

}

void mos6502::INC(uint16_t addr) {

    byte m = this->Read(addr);
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

    byte m = this->Read(addr);
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

    byte m = this->Read(addr);
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
    TOGGLE_FLAG(((this->A >> 8) == (m >> 8) && (m >> 8) != (res >> 8)),flags::V);
    TOGGLE_FLAG(res & 0x80,flags::N);
    TOGGLE_FLAG(!res, flags::Z);
    this->A = res & 0xFF;

}

void mos6502::SBC(uint16_t addr) {


    byte m = this->Read(addr);
    uint16_t res = this->A - m - IS_SET(flags::C);
    

    if(IS_SET(flags::D)) {

        if(((this->A & 0x0F) - IS_SET(flags::C)) < (m & 0x0F)) res -= 6;
        if(res > 0x99) res -= 0x60;

    }

    TOGGLE_FLAG(res < 0x100,flags::C);
    // evaluates if the sign bit of the result is opposite of the operands
    TOGGLE_FLAG(((this->A >> 8) == (m >> 8) && (m >> 8) != (res >> 8)),flags::V);
    TOGGLE_FLAG(res & 0x80,flags::N);
    TOGGLE_FLAG(!res, flags::Z);
    this->A = res & 0xFF;
}

void mos6502::ASL(uint16_t addr) {

    byte m = this->Read(addr);
    TOGGLE_FLAG(m & 0x80,flags::C);
    m = m << 1;

    Write(m,addr);

    TOGGLE_FLAG(m & 0x80, flags::N);
    TOGGLE_FLAG(!m, flags::Z);

}

void mos6502::LSR(uint16_t addr) {

    byte m = this->Read(addr);
    TOGGLE_FLAG(m & 0x01,flags::C);
    m = m >> 1;

    Write(m,addr);

    TOGGLE_FLAG(0,flags::N);
    TOGGLE_FLAG(!m, flags::Z);

}

void mos6502::ROL(uint16_t addr) {

    uint16_t m = this->Read(addr);

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

    uint16_t m = this->Read(addr);

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
    
    byte m = this->Read(addr);
    uint16_t res = this->A - m; 
    TOGGLE_FLAG(res <= 0xFF,flags::C);
    TOGGLE_FLAG(res & 0x80,flags::N);
    TOGGLE_FLAG(!(res & 0xFF),flags::Z);

}


void mos6502::CPX(uint16_t addr) {

    byte m = this->Read(addr);
    uint16_t res = this->X - m; 
    TOGGLE_FLAG(res <= 0xFF,flags::C);
    TOGGLE_FLAG(res & 0x80,flags::N);
    TOGGLE_FLAG(!(res & 0xFF),flags::Z);

}

void mos6502::CPY(uint16_t addr) {

    byte m = this->Read(addr);
    uint16_t res = this->Y - m; 
    TOGGLE_FLAG(res <= 0xFF,flags::C);
    TOGGLE_FLAG(res & 0x80,flags::N);
    TOGGLE_FLAG(!(res & 0xFF),flags::Z);

}

void mos6502::BCC(uint16_t addr) {

    if(!(IS_SET(flags::C))) this->pc = addr; 

}

void mos6502::BCS(uint16_t addr) {

    if(IS_SET(flags::C)) this->pc = addr; 

}

void mos6502::BEQ(uint16_t addr) {

    if(IS_SET(flags::Z)) this->pc = addr; 

}

void mos6502::BMI(uint16_t addr) {

    if(IS_SET(flags::N)) this->pc = addr; 

}

void mos6502::BNE(uint16_t addr) {

    if(!(IS_SET(flags::Z))) this->pc = addr; 

}

void mos6502::BPL(uint16_t addr) {

    if(!(IS_SET(flags::N))) this->pc = addr; 

}

void mos6502::BVC(uint16_t addr) {

    if(!(IS_SET(flags::V))) this->pc = addr; 

}

void mos6502::BVS(uint16_t addr) {

    if(IS_SET(flags::V)) this->pc = addr; 

}


void mos6502::JMP(uint16_t addr) {

    this->pc = addr;

}

void mos6502::JSR(uint16_t addr) {

    this->pc--;
    this->Push((this->pc >> 8) & 0xFF);
    this->Push(this->pc & 0xFF);
    this->pc = addr;

}

void mos6502::RTS(uint16_t addr) {

    uint16_t low_nibble = this->Pop();
    uint16_t high_nibble = this->Pop();
    high_nibble <<= 8;
    
    this->pc = (high_nibble | low_nibble) + 1;

}

void mos6502::BRK(uint16_t addr) {
    

    this->Push((this->pc + 2) >> 8);
    this->Push((this->pc + 2) & 0xFF);
    this->Push(this->status | flags::B);
    TOGGLE_FLAG(1,flags::I);

    uint16_t low_nibble =  this->Read(0xFFFE);
    uint16_t high_nibble = (this->Read(0xFFFF) << 8);

    this->pc = high_nibble | low_nibble;

}

void mos6502::RTI(uint16_t addr) {

    this->status = (this->Pop() ^ flags::B) ^ flags::R;

    uint16_t low_nibble  = this->Pop();
    uint16_t high_nibble = (this->Pop() << 8);

    this->pc = high_nibble | low_nibble;
}

void mos6502::BIT(uint16_t addr) {
    
    byte m = this->Read(addr);
    byte res = this->A & m;
    TOGGLE_FLAG(res & 0x80,flags::N);
    TOGGLE_FLAG(res & flags::V,flags::V);
    TOGGLE_FLAG(!res,flags::Z);

}

void mos6502::NOP(uint16_t addr) {

    return;

}
