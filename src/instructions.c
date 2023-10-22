#include "instructions.h"
#include "types.h"
#include "cpu.h"
#include "macros.h"


void LDA(uint16_t addr, mos6502* cpu) {
    
    byte m = cpu->Read(addr);

    cpu->A = m;
    TOGGLE_FLAG(cpu->A & 0x80, N);
    TOGGLE_FLAG(!cpu->A, Z);

}

void LDX(uint16_t addr, mos6502* cpu) {
    
    byte m = cpu->Read(addr);

    cpu->X = m;
    TOGGLE_FLAG(cpu->X & 0x80, N);
    TOGGLE_FLAG(!cpu->X, Z);

}

void LDY(uint16_t addr, mos6502* cpu) {
    
    byte m = cpu->Read(addr);

    cpu->Y = m;
    TOGGLE_FLAG(cpu->Y & 0x80, N);
    TOGGLE_FLAG(!cpu->Y, Z);

}

void STA(uint16_t addr, mos6502* cpu) {
    cpu->Write(cpu->A,addr);
}

void STX(uint16_t addr, mos6502* cpu) {
    cpu->Write(cpu->X,addr);
}

void STY(uint16_t addr, mos6502* cpu) {
    cpu->Write(cpu->Y,addr);
}

void TAX(uint16_t addr, mos6502* cpu) {
    
    cpu->X = cpu->A;
    TOGGLE_FLAG(cpu->X & 0x80, N);
    TOGGLE_FLAG(!cpu->X, Z);

}

void TAY(uint16_t addr, mos6502* cpu) {
    

    cpu->Y = cpu->A;
    TOGGLE_FLAG(cpu->Y & 0x80, N);
    TOGGLE_FLAG(!cpu->Y, Z);

}

void TSX(uint16_t addr, mos6502* cpu) {
    

    cpu->X = cpu->sp;
    TOGGLE_FLAG(cpu->X & 0x80, N);
    TOGGLE_FLAG(!cpu->X, Z);

}

void TXA(uint16_t addr, mos6502* cpu) {
    

    cpu->A = cpu->X;
    TOGGLE_FLAG(cpu->A & 0x80, N);
    TOGGLE_FLAG(!cpu->A, Z);

}

void TXS(uint16_t addr, mos6502* cpu) {
    
    cpu->sp = cpu->X;

}

void TYA(uint16_t addr, mos6502* cpu) {
    

    cpu->A = cpu->Y;
    TOGGLE_FLAG(cpu->A & 0x80, N);
    TOGGLE_FLAG(!cpu->A, Z);

}

void PHA(uint16_t addr, mos6502* cpu) {
    
    Push(cpu->A,cpu);
}

void PHP(uint16_t addr, mos6502* cpu) {
    
    Push(cpu->status | B | R,cpu);

}

void PLA(uint16_t addr, mos6502* cpu) {
    
    cpu->A = Pop(cpu);
    TOGGLE_FLAG(cpu->A & 0x80, N);
    TOGGLE_FLAG(!cpu->A, Z);


}

void PLP(uint16_t addr, mos6502* cpu) {

    cpu->status = Pop(cpu) ^ B ^ R;

}

void AND(uint16_t addr, mos6502* cpu) {
    
    byte m = cpu->Read(addr);

    cpu->A &= m;
    TOGGLE_FLAG(cpu->A & 0x80, N);
    TOGGLE_FLAG(!cpu->A, Z);

}

void EOR(uint16_t addr, mos6502* cpu) {

    byte m = cpu->Read(addr);

    cpu->A ^= m;
    TOGGLE_FLAG(cpu->A & 0x80, N);
    TOGGLE_FLAG(!cpu->A, Z);

}

void ORA(uint16_t addr, mos6502* cpu) {

    byte m = cpu->Read(addr);
    cpu->A |= m;

    TOGGLE_FLAG(cpu->A & 0x80, N);
    TOGGLE_FLAG(!cpu->A, Z);

}

void INC(uint16_t addr, mos6502* cpu) {

    byte m = cpu->Read(addr);
    m++;
    cpu->Write(m,addr);

    TOGGLE_FLAG(m & 0x80, N);
    TOGGLE_FLAG(!m, Z);

}

void INY(uint16_t addr, mos6502* cpu) {

    cpu->Y++;

    TOGGLE_FLAG(cpu->Y & 0x80, N);
    TOGGLE_FLAG(!cpu->Y, Z);

}

void INX(uint16_t addr, mos6502* cpu) {

    cpu->X++;

    TOGGLE_FLAG(cpu->X & 0x80, N);
    TOGGLE_FLAG(!cpu->X, Z);

}

void DEC(uint16_t addr, mos6502* cpu) {

    byte m = cpu->Read(addr);
    m--;
    cpu->Write(m,addr);

    TOGGLE_FLAG(m & 0x80, N);
    TOGGLE_FLAG(!m, Z);

}

void DEY(uint16_t addr, mos6502* cpu) {

    cpu->Y--;

    TOGGLE_FLAG(cpu->Y & 0x80, N);
    TOGGLE_FLAG(!cpu->Y, Z);

}

void DEX(uint16_t addr, mos6502* cpu) {

    cpu->X--;

    TOGGLE_FLAG(cpu->X & 0x80, N);
    TOGGLE_FLAG(!cpu->X, Z);

}

void ADC(uint16_t addr, mos6502* cpu) {

    byte m = cpu->Read(addr);
    uint16_t res = cpu->A + m + IS_SET(C);
    

    if(IS_SET(D)) {

        byte low_nibble  = (cpu->A & 0x0F) + (m & 0x0F) + IS_SET(C);     
        if(low_nibble > 9) res += 6;
        if(res > 0x99) res += 0x60;

        TOGGLE_FLAG(res > 0x99,C);

    }
    else {

        TOGGLE_FLAG(res > 0xFF,C);

    }

    // evaluates if the sign bit of the result is opposite of the operands
    TOGGLE_FLAG(((cpu->A >> 8) == (m >> 8) && (m >> 8) != (res >> 8)),V);
    TOGGLE_FLAG(res & 0x80,N);
    TOGGLE_FLAG(!res, Z);
    cpu->A = res & 0xFF;

}

void SBC(uint16_t addr, mos6502* cpu) {


    byte m = cpu->Read(addr);
    uint16_t res = cpu->A - m - IS_SET(C);
    

    if(IS_SET(D)) {

        if(((cpu->A & 0x0F) - IS_SET(C)) < (m & 0x0F)) res -= 6;
        if(res > 0x99) res -= 0x60;

    }

    TOGGLE_FLAG(res < 0x100,C);
    // evaluates if the sign bit of the result is opposite of the operands
    TOGGLE_FLAG(((cpu->A >> 8) == (m >> 8) && (m >> 8) != (res >> 8)),V);
    TOGGLE_FLAG(res & 0x80,N);
    TOGGLE_FLAG(!res, Z);
    cpu->A = res & 0xFF;
}

void ASL(uint16_t addr, mos6502* cpu) {

    byte m = cpu->Read(addr);
    TOGGLE_FLAG(m & 0x80,C);
    m = m << 1;

    cpu->Write(m,addr);

    TOGGLE_FLAG(m & 0x80, N);
    TOGGLE_FLAG(!m, Z);

}

void LSR(uint16_t addr, mos6502* cpu) {

    byte m = cpu->Read(addr);
    TOGGLE_FLAG(m & 0x01,C);
    m = m >> 1;

    cpu->Write(m,addr);

    TOGGLE_FLAG(0,N);
    TOGGLE_FLAG(!m, Z);

}

void ROL(uint16_t addr, mos6502* cpu) {

    uint16_t m = cpu->Read(addr);

    if(IS_SET(C)) {
        m |= 0x01;
    }

    m = m << 1;
    TOGGLE_FLAG(m > 0xFF,C);

    m &= 0xFF;
    cpu->Write(m,addr);

    TOGGLE_FLAG(m & 0x80, N);
    TOGGLE_FLAG(!m, Z);

}

void ROR(uint16_t addr, mos6502* cpu) {

    uint16_t m = cpu->Read(addr);

    if(IS_SET(C)) {
        m |= 0x100;
    }

    m = m >> 1;
    TOGGLE_FLAG(m & 0x01,C);

    m &= 0xFF;
    cpu->Write(m,addr);

    TOGGLE_FLAG(m & 0x80, N);
    TOGGLE_FLAG(!m, Z);

}

void ASL_A(uint16_t addr, mos6502* cpu) {

    byte m = cpu->A;
    TOGGLE_FLAG(m & 0x80,C);
    m = m << 1;

    cpu->A = m;

    TOGGLE_FLAG(m & 0x80, N);
    TOGGLE_FLAG(!m, Z);

}

void LSR_A(uint16_t addr, mos6502* cpu) {

    byte m = cpu->A;
    TOGGLE_FLAG(m & 0x01,C);
    m = m >> 1;

    cpu->A = m;

    TOGGLE_FLAG(0,N);
    TOGGLE_FLAG(!m, Z);

}

void ROL_A(uint16_t addr, mos6502* cpu) {

    uint16_t m = cpu->A;

    if(IS_SET(C)) {
        m |= 0x01;
    }

    m = m << 1;
    TOGGLE_FLAG(m > 0xFF,C);

    m &= 0xFF;
    cpu->A = m;

    TOGGLE_FLAG(m & 0x80, N);
    TOGGLE_FLAG(!m, Z);

}

void ROR_A(uint16_t addr, mos6502* cpu) {

    uint16_t m = cpu->A;

    if(IS_SET(C)) {
        m |= 0x100;
    }

    m = m >> 1;
    TOGGLE_FLAG(m & 0x01,C);

    m &= 0xFF;
    cpu->A = m;

    TOGGLE_FLAG(m & 0x80, N);
    TOGGLE_FLAG(!m, Z);

}

void CLC(uint16_t addr, mos6502* cpu) {

    TOGGLE_FLAG(0,C);

}

void CLD(uint16_t addr, mos6502* cpu) {

    TOGGLE_FLAG(0,D);

}

void CLI(uint16_t addr, mos6502* cpu) {

    TOGGLE_FLAG(0,I);

}

void CLV(uint16_t addr, mos6502* cpu) {

    TOGGLE_FLAG(0,V);

}

void SEC(uint16_t addr, mos6502* cpu) {

    TOGGLE_FLAG(1,C);

}

void SED(uint16_t addr, mos6502* cpu) {

    TOGGLE_FLAG(1,D);

}

void SEI(uint16_t addr, mos6502* cpu) {

    TOGGLE_FLAG(1,I);

}

void CMP(uint16_t addr, mos6502* cpu) {
    
    byte m = cpu->Read(addr);
    uint16_t res = cpu->A - m; 
    TOGGLE_FLAG(res <= 0xFF,C);
    TOGGLE_FLAG(res & 0x80,N);
    TOGGLE_FLAG(!(res & 0xFF),Z);

}


void CPX(uint16_t addr, mos6502* cpu) {

    byte m = cpu->Read(addr);
    uint16_t res = cpu->X - m; 
    TOGGLE_FLAG(res <= 0xFF,C);
    TOGGLE_FLAG(res & 0x80,N);
    TOGGLE_FLAG(!(res & 0xFF),Z);

}

void CPY(uint16_t addr, mos6502* cpu) {

    byte m = cpu->Read(addr);
    uint16_t res = cpu->Y - m; 
    TOGGLE_FLAG(res <= 0xFF,C);
    TOGGLE_FLAG(res & 0x80,N);
    TOGGLE_FLAG(!(res & 0xFF),Z);

}

void BCC(uint16_t addr, mos6502* cpu) {

    if(!(IS_SET(C))) cpu->pc = addr; 

}

void BCS(uint16_t addr, mos6502* cpu) {

    if(IS_SET(C)) cpu->pc = addr; 

}

void BEQ(uint16_t addr, mos6502* cpu) {

    if(IS_SET(Z)) cpu->pc = addr; 

}

void BMI(uint16_t addr, mos6502* cpu) {

    if(IS_SET(N)) cpu->pc = addr; 

}

void BNE(uint16_t addr, mos6502* cpu) {

    if(!(IS_SET(Z))) cpu->pc = addr; 

}

void BPL(uint16_t addr, mos6502* cpu) {

    if(!(IS_SET(N))) cpu->pc = addr; 

}

void BVC(uint16_t addr, mos6502* cpu) {

    if(!(IS_SET(V))) cpu->pc = addr; 

}

void BVS(uint16_t addr, mos6502* cpu) {

    if(IS_SET(V)) cpu->pc = addr; 

}


void JMP(uint16_t addr, mos6502* cpu) {

    cpu->pc = addr;

}

void JSR(uint16_t addr, mos6502* cpu) {

    cpu->pc--;
    Push((cpu->pc >> 8) & 0xFF,cpu);
    Push(cpu->pc & 0xFF,cpu);
    cpu->pc = addr;

}

void RTS(uint16_t addr, mos6502* cpu) {

    uint16_t low_nibble = Pop(cpu);
    uint16_t high_nibble = Pop(cpu);
    high_nibble <<= 8;
    
    cpu->pc = (high_nibble | low_nibble) + 1;

}

void BRK(uint16_t addr, mos6502* cpu) {
    

    Push((cpu->pc + 2) >> 8,cpu);
    Push((cpu->pc + 2) & 0xFF,cpu);
    Push(cpu->status | B,cpu);
    TOGGLE_FLAG(1,I);

    uint16_t low_nibble =  cpu->Read(0xFFFE);
    uint16_t high_nibble = (cpu->Read(0xFFFF) << 8);

    cpu->pc = high_nibble | low_nibble;
}

void RTI(uint16_t addr, mos6502* cpu) {

    cpu->status = (Pop(cpu) ^ B) ^ R;

    uint16_t low_nibble  = Pop(cpu);
    uint16_t high_nibble = (Pop(cpu) << 8);

    cpu->pc = high_nibble | low_nibble;
}

void BIT(uint16_t addr, mos6502* cpu) {
    
    byte m = cpu->Read(addr);
    byte res = cpu->A & m;
    TOGGLE_FLAG(res & 0x80,N);
    TOGGLE_FLAG(res & V,V);
    TOGGLE_FLAG(!res,Z);

}

void NOP(uint16_t addr, mos6502* cpu) {

    return;

}
