#include <stdio.h>
#include <malloc.h>
#include <CPU6502/Internal/addressmodes.h>
#include <CPU6502/Internal/instructions.h>
#include <CPU6502/Internal/types.h>
#include <CPU6502/cpu.h>
#include <CPU6502/Internal/macros.h>

void Execute(Instruction i,mos6502* cpu) {
    uint16_t addr = i.mode(cpu);
    i.code(addr,cpu);
}

void Run(mos6502* cpu) {

    Instruction ins;
    byte opcode;

    while(1) {

        opcode = cpu->Read(cpu->pc++);

        ins = cpu->table[opcode];

        Execute(ins,cpu);

    }

}

void NMI(mos6502* cpu) {
    
    Push(cpu->pc >> 8, cpu);
    Push(cpu->pc & 0xFF, cpu);
    Push(cpu->status, cpu);

    TOGGLE_FLAG(1,I);

    uint16_t low_nibble =  cpu->Read(0xFFFA);
    uint16_t high_nibble = (cpu->Read(0xFFFB) << 8);

    cpu->pc = high_nibble | low_nibble;

}

void IRQ(mos6502* cpu) {
        
    if(!(IS_SET(I))) {

        Push(cpu->pc >> 8, cpu);
        Push(cpu->pc & 0xFF, cpu);
        Push(cpu->status, cpu);

        TOGGLE_FLAG(1,I);

        uint16_t low_nibble =  cpu->Read(0xFFFE);
        uint16_t high_nibble = (cpu->Read(0xFFFF) << 8);

        cpu->pc = high_nibble | low_nibble;
    }

}

void Reset(mos6502* cpu) {

    TOGGLE_FLAG(1, I);
    
    uint16_t low_nibble  = cpu->Read(0xFFFC);
    uint16_t high_nibble = (cpu->Read(0xFFFD) << 8);

    cpu->pc = high_nibble | low_nibble;

}

void Push(byte m,mos6502* cpu) {

    cpu->Write(m,0x0100 + cpu->sp);
    cpu->sp = cpu->sp == 0x00 ? 0xFF : cpu->sp - 1;

}

byte Pop(mos6502* cpu) {

    cpu->sp = cpu->sp == 0xFF ? 0x00 : cpu->sp + 1;

    return cpu->Read(0x0100 + cpu->sp);

}

mos6502* createCpu() {

    mos6502* cpu = malloc(sizeof(mos6502));

    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    cpu->pc = 0;
    cpu->status = 0;
    cpu->sp = 0;

    cpu->table[0x69].mode = &ImmediateMode;
    cpu->table[0x69].code = &ADC;
    cpu->table[0x65].mode = &ZeroPageMode ; 
    cpu->table[0x65].code = &ADC;
    cpu->table[0x75].mode = &ZeroPageXMode ;
    cpu->table[0x75].code = &ADC;
    cpu->table[0x6D].mode = &AbsoluteMode ;
    cpu->table[0x6D].code = &ADC;
    cpu->table[0x7D].mode = &AbsoluteXMode ;
    cpu->table[0x7D].code = &ADC;
    cpu->table[0x79].mode = &AbsoluteYMode ;
    cpu->table[0x79].code = &ADC;
    cpu->table[0x61].mode = &IndirectXMode ;
    cpu->table[0x61].code = &ADC;
    cpu->table[0x71].mode = &IndirectYMode ;
    cpu->table[0x71].code = &ADC;
    
    cpu->table[0x29].mode = &ImmediateMode ;
    cpu->table[0x29].code = &AND;
    cpu->table[0x25].mode = &ZeroPageMode ;
    cpu->table[0x25].code = &AND;
    cpu->table[0x35].mode = &ZeroPageXMode ;
    cpu->table[0x35].code = &AND;
    cpu->table[0x2D].mode = &AbsoluteMode ;
    cpu->table[0x2D].code = &AND;
    cpu->table[0x3D].mode = &AbsoluteXMode ;
    cpu->table[0x3D].code = &AND;
    cpu->table[0x39].mode = &AbsoluteYMode ;
    cpu->table[0x39].code = &AND;
    cpu->table[0x21].mode = &IndirectXMode ;
    cpu->table[0x21].code = &AND;
    cpu->table[0x31].mode = &IndirectYMode ;
    cpu->table[0x31].code = &AND;
    
    cpu->table[0x0A].mode = &AccumulatorMode;
    cpu->table[0x0A].code = &ASL_A;
    cpu->table[0x06].mode = &ZeroPageMode ;
    cpu->table[0x06].code = &ASL;
    cpu->table[0x16].mode = &ZeroPageXMode ;
    cpu->table[0x16].code = &ASL;
    cpu->table[0x0E].mode = &AbsoluteMode ;
    cpu->table[0x0E].code = &ASL;
    cpu->table[0x1E].mode = &AbsoluteXMode ;
    cpu->table[0x1E].code = &ASL;
    
    cpu->table[0x24].mode = &ZeroPageMode ;
    cpu->table[0x24].code = &BIT;
    cpu->table[0x2C].mode = &AbsoluteMode ;
    cpu->table[0x2C].code = &BIT;
    
    cpu->table[0x00].mode = &ImpliedMode ;
    cpu->table[0x00].code = &BRK;
    
    cpu->table[0x18].mode = &ImpliedMode ;
    cpu->table[0x18].code = &CLC;
    cpu->table[0xD8].mode = &ImpliedMode ;
    cpu->table[0xD8].code = &CLD;
    cpu->table[0x58].mode = &ImpliedMode ;
    cpu->table[0x58].code = &CLI;
    cpu->table[0xB8].mode = &ImpliedMode ;
    cpu->table[0xB8].code = &CLV;
    
    cpu->table[0xC9].mode = &ImmediateMode ;
    cpu->table[0xC9].code = &CMP;
    cpu->table[0xC5].mode = &ZeroPageMode ;
    cpu->table[0xC5].code = &CMP;
    cpu->table[0xD5].mode = &ZeroPageXMode ;
    cpu->table[0xD5].code = &CMP;
    cpu->table[0xCD].mode = &AbsoluteMode ;
    cpu->table[0xCD].code = &CMP;
    cpu->table[0xDD].mode = &AbsoluteXMode ;
    cpu->table[0xDD].code = &CMP;
    cpu->table[0xD9].mode = &AbsoluteYMode ;
    cpu->table[0xD9].code = &CMP;
    cpu->table[0xC1].mode = &IndirectXMode ;
    cpu->table[0xC1].code = &CMP;
    cpu->table[0xD1].mode = &IndirectYMode ;
    cpu->table[0xD1].code = &CMP;
    
    cpu->table[0xE0].mode = &ImmediateMode ;
    cpu->table[0xE0].code = &CPX;
    cpu->table[0xE4].mode = &ZeroPageMode ;
    cpu->table[0xE4].code = &CPX;
    cpu->table[0xEC].mode = &AbsoluteMode ;
    cpu->table[0xEC].code = &CPX;
    
    cpu->table[0xC0].mode = &ImmediateMode ;
    cpu->table[0xC0].code = &CPY;
    cpu->table[0xC4].mode = &ZeroPageMode  ;
    cpu->table[0xC4].code = &CPY;
    cpu->table[0xCC].mode = &AbsoluteMode  ;
    cpu->table[0xCC].code = &CPY;
    
    cpu->table[0xC6].mode = &ZeroPageMode ;
    cpu->table[0xC6].code = &DEC;
    cpu->table[0xD6].mode = &ZeroPageXMode ;
    cpu->table[0xD6].code = &DEC;
    cpu->table[0xCE].mode = &AbsoluteMode ;
    cpu->table[0xCE].code = &DEC;
    cpu->table[0xDE].mode = &AbsoluteXMode ;
    cpu->table[0xDE].code = &DEC;
    
    cpu->table[0xE6].mode = &ZeroPageMode ;
    cpu->table[0xE6].code = &INC;
    cpu->table[0xF6].mode = &ZeroPageXMode ;
    cpu->table[0xF6].code = &INC;
    cpu->table[0xEE].mode = &AbsoluteMode ;
    cpu->table[0xEE].code = &INC;
    cpu->table[0xFE].mode = &AbsoluteXMode ;
    cpu->table[0xFE].code = &INC;
    
    cpu->table[0xCA].mode = &ImpliedMode ;
    cpu->table[0xCA].code = &DEX;
    cpu->table[0x88].mode = &ImpliedMode ;
    cpu->table[0x88].code = &DEY;
    cpu->table[0xE8].mode = &ImpliedMode ;
    cpu->table[0xE8].code = &INX;
    cpu->table[0xC8].mode = &ImpliedMode ;
    cpu->table[0xC8].code = &INY;
    
    cpu->table[0x49].mode = &ImmediateMode ;
    cpu->table[0x49].code = &EOR;
    cpu->table[0x45].mode = &ZeroPageMode ;
    cpu->table[0x45].code = &EOR;
    cpu->table[0x55].mode = &ZeroPageXMode ;
    cpu->table[0x55].code = &EOR;
    cpu->table[0x4D].mode = &AbsoluteMode ;
    cpu->table[0x4D].code = &EOR;
    cpu->table[0x5D].mode = &AbsoluteXMode ;
    cpu->table[0x5D].code = &EOR;
    cpu->table[0x59].mode = &AbsoluteYMode ;
    cpu->table[0x59].code = &EOR;
    cpu->table[0x41].mode = &IndirectXMode ;
    cpu->table[0x41].code = &EOR;
    cpu->table[0x51].mode = &IndirectYMode ;
    cpu->table[0x51].code = &EOR;
    
    cpu->table[0x4C].mode = &AbsoluteMode ;
    cpu->table[0x4C].code = &JMP;
    cpu->table[0x6C].mode = &IndirectMode ;
    cpu->table[0x6C].code = &JMP;
    
    cpu->table[0x20].mode = &AbsoluteMode ;
    cpu->table[0x20].code = &JSR;
    
    cpu->table[0xA9].mode = &ImmediateMode ;
    cpu->table[0xA9].code = &LDA;
    cpu->table[0xA5].mode = &ZeroPageMode ;
    cpu->table[0xA5].code = &LDA;
    cpu->table[0xB5].mode = &ZeroPageXMode ;
    cpu->table[0xB5].code = &LDA;
    cpu->table[0xAD].mode = &AbsoluteMode ;
    cpu->table[0xAD].code = &LDA;
    cpu->table[0xBD].mode = &AbsoluteXMode ;
    cpu->table[0xBD].code = &LDA;
    cpu->table[0xB9].mode = &AbsoluteYMode ;
    cpu->table[0xB9].code = &LDA;
    cpu->table[0xA1].mode = &IndirectXMode ;
    cpu->table[0xA1].code = &LDA;
    cpu->table[0xB1].mode = &IndirectYMode ;
    cpu->table[0xB1].code = &LDA;
    
    cpu->table[0xA2].mode = &ImmediateMode ;
    cpu->table[0xA2].code = &LDX;
    cpu->table[0xA6].mode = &ZeroPageMode ;
    cpu->table[0xA6].code = &LDX;
    cpu->table[0xB6].mode = &ZeroPageYMode ;
    cpu->table[0xB6].code = &LDX;
    cpu->table[0xAE].mode = &AbsoluteMode ;
    cpu->table[0xAE].code = &LDX;
    cpu->table[0xBE].mode = &AbsoluteYMode ;
    cpu->table[0xBE].code = &LDX;
    
    cpu->table[0xA0].mode = &ImmediateMode ;
    cpu->table[0xA0].code = &LDY;
    cpu->table[0xA4].mode = &ZeroPageMode ;
    cpu->table[0xA4].code = &LDY;
    cpu->table[0xB4].mode = &ZeroPageXMode ;
    cpu->table[0xB4].code = &LDY;
    cpu->table[0xAC].mode = &AbsoluteMode ;
    cpu->table[0xAC].code = &LDY;
    cpu->table[0xBC].mode = &AbsoluteXMode ;
    cpu->table[0xBC].code = &LDY;
    
    cpu->table[0x4A].mode = &AccumulatorMode;
    cpu->table[0x4A].code = &LSR_A;
    cpu->table[0x46].mode = &ZeroPageMode ;  
    cpu->table[0x46].code = &LSR;
    cpu->table[0x56].mode = &ZeroPageXMode ; 
    cpu->table[0x56].code = &LSR;
    cpu->table[0x4E].mode = &AbsoluteMode ;  
    cpu->table[0x4E].code = &LSR;
    cpu->table[0x5E].mode = &AbsoluteXMode ;
    cpu->table[0x5E].code = &LSR;
    
    cpu->table[0xEA].mode = &ImpliedMode ;  
    cpu->table[0xEA].code = &NOP;
    
    cpu->table[0x09].mode = &ImmediateMode ; 
    cpu->table[0x09].code = &ORA;
    cpu->table[0x05].mode = &ZeroPageMode ;  
    cpu->table[0x05].code = &ORA;
    cpu->table[0x15].mode = &ZeroPageXMode ; 
    cpu->table[0x15].code = &ORA;
    cpu->table[0x0D].mode = &AbsoluteMode ;  
    cpu->table[0x0D].code = &ORA;
    cpu->table[0x1D].mode = &AbsoluteXMode ; 
    cpu->table[0x1D].code = &ORA;
    cpu->table[0x19].mode = &AbsoluteYMode ; 
    cpu->table[0x19].code = &ORA;
    cpu->table[0x01].mode = &IndirectXMode ; 
    cpu->table[0x01].code = &ORA;
    cpu->table[0x11].mode = &IndirectYMode ; 
    cpu->table[0x11].code = &ORA;
    
    cpu->table[0x48].mode = &ImpliedMode ; 
    cpu->table[0x48].code = &PHA;
    cpu->table[0x08].mode = &ImpliedMode ;
    cpu->table[0x08].code = &PHP;
    cpu->table[0x68].mode = &ImpliedMode ;
    cpu->table[0x68].code = &PLA;
    cpu->table[0x28].mode = &ImpliedMode ;
    cpu->table[0x28].code = &PLA;
    
    cpu->table[0x2A].mode = &AccumulatorMode;
    cpu->table[0x2A].code = &ROL_A;
    cpu->table[0x26].mode = &ZeroPageMode ;  
    cpu->table[0x26].code = &ROL;
    cpu->table[0x36].mode = &ZeroPageXMode ; 
    cpu->table[0x36].code = &ROL;
    cpu->table[0x2E].mode = &AbsoluteMode ;  
    cpu->table[0x2E].code = &ROL;
    cpu->table[0x3E].mode = &AbsoluteXMode ; 
    cpu->table[0x3E].code = &ROL;
    
    cpu->table[0x6A].mode = &AccumulatorMode;
    cpu->table[0x6A].code = &ROR_A;
    cpu->table[0x66].mode = &ZeroPageMode ;  
    cpu->table[0x66].code = &ROR;
    cpu->table[0x76].mode = &ZeroPageXMode ; 
    cpu->table[0x76].code = &ROR;
    cpu->table[0x6E].mode = &AbsoluteMode ;  
    cpu->table[0x6E].code = &ROR;
    cpu->table[0x7E].mode = &AbsoluteXMode ; 
    cpu->table[0x7E].code = &ROR;
    
    cpu->table[0x40].mode = &ImpliedMode ; 
    cpu->table[0x40].code = &RTI;
    
    cpu->table[0x60].mode = &ImpliedMode ;   
    cpu->table[0x60].code = &RTS;
    
    cpu->table[0xE9].mode = &ImmediateMode ; 
    cpu->table[0xE9].code = &SBC;
    cpu->table[0xE5].mode = &ZeroPageMode ; 
    cpu->table[0xE5].code = &SBC;
    cpu->table[0xF5].mode = &ZeroPageXMode ;
    cpu->table[0xF5].code = &SBC;
    cpu->table[0xED].mode = &AbsoluteMode ; 
    cpu->table[0xED].code = &SBC;
    cpu->table[0xFD].mode = &AbsoluteXMode ;
    cpu->table[0xFD].code = &SBC;
    cpu->table[0xF9].mode = &AbsoluteYMode ;
    cpu->table[0xF9].code = &SBC;
    cpu->table[0xE1].mode = &IndirectXMode ;
    cpu->table[0xE1].code = &SBC;
    cpu->table[0xF1].mode = &IndirectYMode ;
    cpu->table[0xF1].code = &SBC;
    
    cpu->table[0x38].mode = &ImpliedMode ;   
    cpu->table[0x38].code = &SEC;
    cpu->table[0xF8].mode = &ImpliedMode ;   
    cpu->table[0xF8].code = &SED;
    cpu->table[0x78].mode = &ImpliedMode ;   
    cpu->table[0x78].code = &SEI;
    
    cpu->table[0x85].mode = &ZeroPageMode ;  
    cpu->table[0x85].code = &STA;    
    cpu->table[0x95].mode = &ZeroPageXMode ; 
    cpu->table[0x95].code = &STA;    
    cpu->table[0x8D].mode = &AbsoluteMode ;  
    cpu->table[0x8D].code = &STA;    
    cpu->table[0x9D].mode = &AbsoluteXMode ; 
    cpu->table[0x9D].code = &STA;    
    cpu->table[0x99].mode = &AbsoluteYMode ; 
    cpu->table[0x99].code = &STA;    
    cpu->table[0x81].mode = &IndirectXMode ; 
    cpu->table[0x81].code = &STA;    
    cpu->table[0x91].mode = &IndirectYMode ; 
    cpu->table[0x91].code = &STA;    

    cpu->table[0x86].mode = &ZeroPageMode ;  
    cpu->table[0x86].code = &STX;
    cpu->table[0x96].mode = &ZeroPageYMode ; 
    cpu->table[0x96].code = &STX;
    cpu->table[0x8E].mode = &AbsoluteMode ;  
    cpu->table[0x8E].code = &STX;    

    cpu->table[0x84].mode = &ZeroPageMode ;  
    cpu->table[0x84].code = &STY;
    cpu->table[0x94].mode = &ZeroPageXMode ; 
    cpu->table[0x94].code = &STY;
    cpu->table[0x8C].mode = &AbsoluteMode ;  
    cpu->table[0x8C].code = &STY;    

    cpu->table[0xAA].mode = &ImpliedMode ;
    cpu->table[0xAA].code = &TAX;
    cpu->table[0xA8].mode = &ImpliedMode ;
    cpu->table[0xA8].code = &TAY;
    cpu->table[0xBA].mode = &ImpliedMode ;
    cpu->table[0xBA].code = &TSX;
    cpu->table[0x8A].mode = &ImpliedMode ;
    cpu->table[0x8A].code = &TXA;
    cpu->table[0x9A].mode = &ImpliedMode ;
    cpu->table[0x9A].code = &TXS;
    cpu->table[0x98].mode = &ImpliedMode ;
    cpu->table[0x98].code = &TYA;
    
    cpu->table[0x90].mode = &RelativeMode ; 
    cpu->table[0x90].code = &BCC;
    cpu->table[0xB0].mode = &RelativeMode ;
    cpu->table[0xB0].code = &BCS;
    cpu->table[0xF0].mode = &RelativeMode ;
    cpu->table[0xF0].code = &BEQ;
    cpu->table[0x30].mode = &RelativeMode ;
    cpu->table[0x30].code = &BMI;
    cpu->table[0xD0].mode = &RelativeMode ;
    cpu->table[0xD0].code = &BNE;
    cpu->table[0x10].mode = &RelativeMode ;
    cpu->table[0x10].code = &BPL;
    cpu->table[0x50].mode = &RelativeMode ;
    cpu->table[0x50].code = &BVC;
    cpu->table[0x70].mode = &RelativeMode ;
    cpu->table[0x70].code = &BVS;

    return cpu;
}
