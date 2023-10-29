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

void Run(mos6502* cpu,void (*callback)()) {

    Instruction ins;
    byte opcode;

    while(1) {

        opcode = cpu->Read(cpu->pc++);

        ins = cpu->table[opcode];

        Execute(ins,cpu);

        if(callback) callback();

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
    cpu->sp--;

}

byte Pop(mos6502* cpu) {

    cpu->sp++;
    return cpu->Read(0x0100 + cpu->sp);

}

mos6502* createCpu() {

    mos6502* newCpu = malloc(sizeof(mos6502));

    newCpu->A = 0;
    newCpu->X = 0;
    newCpu->Y = 0;
    newCpu->pc = 0;
    newCpu->status = 0;
    newCpu->sp = 0;
    for (int i = 0; i < 256; i++) {
        newCpu->table[i].mode = &ImpliedMode;
        newCpu->table[i].code = &NOP;
    }

    newCpu->table[0x69].mode = &ImmediateMode;
    newCpu->table[0x69].code = &ADC;
    newCpu->table[0x65].mode = &ZeroPageMode ; 
    newCpu->table[0x65].code = &ADC;
    newCpu->table[0x75].mode = &ZeroPageXMode ;
    newCpu->table[0x75].code = &ADC;
    newCpu->table[0x6D].mode = &AbsoluteMode ;
    newCpu->table[0x6D].code = &ADC;
    newCpu->table[0x7D].mode = &AbsoluteXMode ;
    newCpu->table[0x7D].code = &ADC;
    newCpu->table[0x79].mode = &AbsoluteYMode ;
    newCpu->table[0x79].code = &ADC;
    newCpu->table[0x61].mode = &IndirectXMode ;
    newCpu->table[0x61].code = &ADC;
    newCpu->table[0x71].mode = &IndirectYMode ;
    newCpu->table[0x71].code = &ADC;
    
    newCpu->table[0x29].mode = &ImmediateMode ;
    newCpu->table[0x29].code = &AND;
    newCpu->table[0x25].mode = &ZeroPageMode ;
    newCpu->table[0x25].code = &AND;
    newCpu->table[0x35].mode = &ZeroPageXMode ;
    newCpu->table[0x35].code = &AND;
    newCpu->table[0x2D].mode = &AbsoluteMode ;
    newCpu->table[0x2D].code = &AND;
    newCpu->table[0x3D].mode = &AbsoluteXMode ;
    newCpu->table[0x3D].code = &AND;
    newCpu->table[0x39].mode = &AbsoluteYMode ;
    newCpu->table[0x39].code = &AND;
    newCpu->table[0x21].mode = &IndirectXMode ;
    newCpu->table[0x21].code = &AND;
    newCpu->table[0x31].mode = &IndirectYMode ;
    newCpu->table[0x31].code = &AND;
    
    newCpu->table[0x0A].mode = &AccumulatorMode;
    newCpu->table[0x0A].code = &ASL_A;
    newCpu->table[0x06].mode = &ZeroPageMode ;
    newCpu->table[0x06].code = &ASL;
    newCpu->table[0x16].mode = &ZeroPageXMode ;
    newCpu->table[0x16].code = &ASL;
    newCpu->table[0x0E].mode = &AbsoluteMode ;
    newCpu->table[0x0E].code = &ASL;
    newCpu->table[0x1E].mode = &AbsoluteXMode ;
    newCpu->table[0x1E].code = &ASL;
    
    newCpu->table[0x24].mode = &ZeroPageMode ;
    newCpu->table[0x24].code = &BIT;
    newCpu->table[0x2C].mode = &AbsoluteMode ;
    newCpu->table[0x2C].code = &BIT;
    
    newCpu->table[0x00].mode = &ImpliedMode ;
    newCpu->table[0x00].code = &BRK;
    
    newCpu->table[0x18].mode = &ImpliedMode ;
    newCpu->table[0x18].code = &CLC;
    newCpu->table[0xD8].mode = &ImpliedMode ;
    newCpu->table[0xD8].code = &CLD;
    newCpu->table[0x58].mode = &ImpliedMode ;
    newCpu->table[0x58].code = &CLI;
    newCpu->table[0xB8].mode = &ImpliedMode ;
    newCpu->table[0xB8].code = &CLV;
    
    newCpu->table[0xC9].mode = &ImmediateMode ;
    newCpu->table[0xC9].code = &CMP;
    newCpu->table[0xC5].mode = &ZeroPageMode ;
    newCpu->table[0xC5].code = &CMP;
    newCpu->table[0xD5].mode = &ZeroPageXMode ;
    newCpu->table[0xD5].code = &CMP;
    newCpu->table[0xCD].mode = &AbsoluteMode ;
    newCpu->table[0xCD].code = &CMP;
    newCpu->table[0xDD].mode = &AbsoluteXMode ;
    newCpu->table[0xDD].code = &CMP;
    newCpu->table[0xD9].mode = &AbsoluteYMode ;
    newCpu->table[0xD9].code = &CMP;
    newCpu->table[0xC1].mode = &IndirectXMode ;
    newCpu->table[0xC1].code = &CMP;
    newCpu->table[0xD1].mode = &IndirectYMode ;
    newCpu->table[0xD1].code = &CMP;
    
    newCpu->table[0xE0].mode = &ImmediateMode ;
    newCpu->table[0xE0].code = &CPX;
    newCpu->table[0xE4].mode = &ZeroPageMode ;
    newCpu->table[0xE4].code = &CPX;
    newCpu->table[0xEC].mode = &AbsoluteMode ;
    newCpu->table[0xEC].code = &CPX;
    
    newCpu->table[0xC0].mode = &ImmediateMode ;
    newCpu->table[0xC0].code = &CPY;
    newCpu->table[0xC4].mode = &ZeroPageMode  ;
    newCpu->table[0xC4].code = &CPY;
    newCpu->table[0xCC].mode = &AbsoluteMode  ;
    newCpu->table[0xCC].code = &CPY;
    
    newCpu->table[0xC6].mode = &ZeroPageMode ;
    newCpu->table[0xC6].code = &DEC;
    newCpu->table[0xD6].mode = &ZeroPageXMode ;
    newCpu->table[0xD6].code = &DEC;
    newCpu->table[0xCE].mode = &AbsoluteMode ;
    newCpu->table[0xCE].code = &DEC;
    newCpu->table[0xDE].mode = &AbsoluteXMode ;
    newCpu->table[0xDE].code = &DEC;
    
    newCpu->table[0xE6].mode = &ZeroPageMode ;
    newCpu->table[0xE6].code = &INC;
    newCpu->table[0xF6].mode = &ZeroPageXMode ;
    newCpu->table[0xF6].code = &INC;
    newCpu->table[0xEE].mode = &AbsoluteMode ;
    newCpu->table[0xEE].code = &INC;
    newCpu->table[0xFE].mode = &AbsoluteXMode ;
    newCpu->table[0xFE].code = &INC;
    
    newCpu->table[0xCA].mode = &ImpliedMode ;
    newCpu->table[0xCA].code = &DEX;
    newCpu->table[0x88].mode = &ImpliedMode ;
    newCpu->table[0x88].code = &DEY;
    newCpu->table[0xE8].mode = &ImpliedMode ;
    newCpu->table[0xE8].code = &INX;
    newCpu->table[0xC8].mode = &ImpliedMode ;
    newCpu->table[0xC8].code = &INY;
    
    newCpu->table[0x49].mode = &ImmediateMode ;
    newCpu->table[0x49].code = &EOR;
    newCpu->table[0x45].mode = &ZeroPageMode ;
    newCpu->table[0x45].code = &EOR;
    newCpu->table[0x55].mode = &ZeroPageXMode ;
    newCpu->table[0x55].code = &EOR;
    newCpu->table[0x4D].mode = &AbsoluteMode ;
    newCpu->table[0x4D].code = &EOR;
    newCpu->table[0x5D].mode = &AbsoluteXMode ;
    newCpu->table[0x5D].code = &EOR;
    newCpu->table[0x59].mode = &AbsoluteYMode ;
    newCpu->table[0x59].code = &EOR;
    newCpu->table[0x41].mode = &IndirectXMode ;
    newCpu->table[0x41].code = &EOR;
    newCpu->table[0x51].mode = &IndirectYMode ;
    newCpu->table[0x51].code = &EOR;
    
    newCpu->table[0x4C].mode = &AbsoluteMode ;
    newCpu->table[0x4C].code = &JMP;
    newCpu->table[0x6C].mode = &IndirectMode ;
    newCpu->table[0x6C].code = &JMP;
    
    newCpu->table[0x20].mode = &AbsoluteMode ;
    newCpu->table[0x20].code = &JSR;
    
    newCpu->table[0xA9].mode = &ImmediateMode ;
    newCpu->table[0xA9].code = &LDA;
    newCpu->table[0xA5].mode = &ZeroPageMode ;
    newCpu->table[0xA5].code = &LDA;
    newCpu->table[0xB5].mode = &ZeroPageXMode ;
    newCpu->table[0xB5].code = &LDA;
    newCpu->table[0xAD].mode = &AbsoluteMode ;
    newCpu->table[0xAD].code = &LDA;
    newCpu->table[0xBD].mode = &AbsoluteXMode ;
    newCpu->table[0xBD].code = &LDA;
    newCpu->table[0xB9].mode = &AbsoluteYMode ;
    newCpu->table[0xB9].code = &LDA;
    newCpu->table[0xA1].mode = &IndirectXMode ;
    newCpu->table[0xA1].code = &LDA;
    newCpu->table[0xB1].mode = &IndirectYMode ;
    newCpu->table[0xB1].code = &LDA;
    
    newCpu->table[0xA2].mode = &ImmediateMode ;
    newCpu->table[0xA2].code = &LDX;
    newCpu->table[0xA6].mode = &ZeroPageMode ;
    newCpu->table[0xA6].code = &LDX;
    newCpu->table[0xB6].mode = &ZeroPageYMode ;
    newCpu->table[0xB6].code = &LDX;
    newCpu->table[0xAE].mode = &AbsoluteMode ;
    newCpu->table[0xAE].code = &LDX;
    newCpu->table[0xBE].mode = &AbsoluteYMode ;
    newCpu->table[0xBE].code = &LDX;
    
    newCpu->table[0xA0].mode = &ImmediateMode ;
    newCpu->table[0xA0].code = &LDY;
    newCpu->table[0xA4].mode = &ZeroPageMode ;
    newCpu->table[0xA4].code = &LDY;
    newCpu->table[0xB4].mode = &ZeroPageXMode ;
    newCpu->table[0xB4].code = &LDY;
    newCpu->table[0xAC].mode = &AbsoluteMode ;
    newCpu->table[0xAC].code = &LDY;
    newCpu->table[0xBC].mode = &AbsoluteXMode ;
    newCpu->table[0xBC].code = &LDY;
    
    newCpu->table[0x4A].mode = &AccumulatorMode;
    newCpu->table[0x4A].code = &LSR_A;
    newCpu->table[0x46].mode = &ZeroPageMode ;  
    newCpu->table[0x46].code = &LSR;
    newCpu->table[0x56].mode = &ZeroPageXMode ; 
    newCpu->table[0x56].code = &LSR;
    newCpu->table[0x4E].mode = &AbsoluteMode ;  
    newCpu->table[0x4E].code = &LSR;
    newCpu->table[0x5E].mode = &AbsoluteXMode ;
    newCpu->table[0x5E].code = &LSR;
    
    newCpu->table[0xEA].mode = &ImpliedMode ;  
    newCpu->table[0xEA].code = &NOP;
    
    newCpu->table[0x09].mode = &ImmediateMode ; 
    newCpu->table[0x09].code = &ORA;
    newCpu->table[0x05].mode = &ZeroPageMode ;  
    newCpu->table[0x05].code = &ORA;
    newCpu->table[0x15].mode = &ZeroPageXMode ; 
    newCpu->table[0x15].code = &ORA;
    newCpu->table[0x0D].mode = &AbsoluteMode ;  
    newCpu->table[0x0D].code = &ORA;
    newCpu->table[0x1D].mode = &AbsoluteXMode ; 
    newCpu->table[0x1D].code = &ORA;
    newCpu->table[0x19].mode = &AbsoluteYMode ; 
    newCpu->table[0x19].code = &ORA;
    newCpu->table[0x01].mode = &IndirectXMode ; 
    newCpu->table[0x01].code = &ORA;
    newCpu->table[0x11].mode = &IndirectYMode ; 
    newCpu->table[0x11].code = &ORA;
    
    newCpu->table[0x48].mode = &ImpliedMode ; 
    newCpu->table[0x48].code = &PHA;
    newCpu->table[0x08].mode = &ImpliedMode ;
    newCpu->table[0x08].code = &PHP;
    newCpu->table[0x68].mode = &ImpliedMode ;
    newCpu->table[0x68].code = &PLA;
    newCpu->table[0x28].mode = &ImpliedMode ;
    newCpu->table[0x28].code = &PLA;
    
    newCpu->table[0x2A].mode = &AccumulatorMode;
    newCpu->table[0x2A].code = &ROL_A;
    newCpu->table[0x26].mode = &ZeroPageMode ;  
    newCpu->table[0x26].code = &ROL;
    newCpu->table[0x36].mode = &ZeroPageXMode ; 
    newCpu->table[0x36].code = &ROL;
    newCpu->table[0x2E].mode = &AbsoluteMode ;  
    newCpu->table[0x2E].code = &ROL;
    newCpu->table[0x3E].mode = &AbsoluteXMode ; 
    newCpu->table[0x3E].code = &ROL;
    
    newCpu->table[0x6A].mode = &AccumulatorMode;
    newCpu->table[0x6A].code = &ROR_A;
    newCpu->table[0x66].mode = &ZeroPageMode ;  
    newCpu->table[0x66].code = &ROR;
    newCpu->table[0x76].mode = &ZeroPageXMode ; 
    newCpu->table[0x76].code = &ROR;
    newCpu->table[0x6E].mode = &AbsoluteMode ;  
    newCpu->table[0x6E].code = &ROR;
    newCpu->table[0x7E].mode = &AbsoluteXMode ; 
    newCpu->table[0x7E].code = &ROR;
    
    newCpu->table[0x40].mode = &ImpliedMode ; 
    newCpu->table[0x40].code = &RTI;
    
    newCpu->table[0x60].mode = &ImpliedMode ;   
    newCpu->table[0x60].code = &RTS;
    
    newCpu->table[0xE9].mode = &ImmediateMode ; 
    newCpu->table[0xE9].code = &SBC;
    newCpu->table[0xE5].mode = &ZeroPageMode ; 
    newCpu->table[0xE5].code = &SBC;
    newCpu->table[0xF5].mode = &ZeroPageXMode ;
    newCpu->table[0xF5].code = &SBC;
    newCpu->table[0xED].mode = &AbsoluteMode ; 
    newCpu->table[0xED].code = &SBC;
    newCpu->table[0xFD].mode = &AbsoluteXMode ;
    newCpu->table[0xFD].code = &SBC;
    newCpu->table[0xF9].mode = &AbsoluteYMode ;
    newCpu->table[0xF9].code = &SBC;
    newCpu->table[0xE1].mode = &IndirectXMode ;
    newCpu->table[0xE1].code = &SBC;
    newCpu->table[0xF1].mode = &IndirectYMode ;
    newCpu->table[0xF1].code = &SBC;
    
    newCpu->table[0x38].mode = &ImpliedMode ;   
    newCpu->table[0x38].code = &SEC;
    newCpu->table[0xF8].mode = &ImpliedMode ;   
    newCpu->table[0xF8].code = &SED;
    newCpu->table[0x78].mode = &ImpliedMode ;   
    newCpu->table[0x78].code = &SEI;
    
    newCpu->table[0x85].mode = &ZeroPageMode ;  
    newCpu->table[0x85].code = &STA;    
    newCpu->table[0x95].mode = &ZeroPageXMode ; 
    newCpu->table[0x95].code = &STA;    
    newCpu->table[0x8D].mode = &AbsoluteMode ;  
    newCpu->table[0x8D].code = &STA;    
    newCpu->table[0x9D].mode = &AbsoluteXMode ; 
    newCpu->table[0x9D].code = &STA;    
    newCpu->table[0x99].mode = &AbsoluteYMode ; 
    newCpu->table[0x99].code = &STA;    
    newCpu->table[0x81].mode = &IndirectXMode ; 
    newCpu->table[0x81].code = &STA;    
    newCpu->table[0x91].mode = &IndirectYMode ; 
    newCpu->table[0x91].code = &STA;    

    newCpu->table[0x86].mode = &ZeroPageMode ;  
    newCpu->table[0x86].code = &STX;
    newCpu->table[0x96].mode = &ZeroPageYMode ; 
    newCpu->table[0x96].code = &STX;
    newCpu->table[0x8E].mode = &AbsoluteMode ;  
    newCpu->table[0x8E].code = &STX;    

    newCpu->table[0x84].mode = &ZeroPageMode ;  
    newCpu->table[0x84].code = &STY;
    newCpu->table[0x94].mode = &ZeroPageXMode ; 
    newCpu->table[0x94].code = &STY;
    newCpu->table[0x8C].mode = &AbsoluteMode ;  
    newCpu->table[0x8C].code = &STY;    

    newCpu->table[0xAA].mode = &ImpliedMode ;
    newCpu->table[0xAA].code = &TAX;
    newCpu->table[0xA8].mode = &ImpliedMode ;
    newCpu->table[0xA8].code = &TAY;
    newCpu->table[0xBA].mode = &ImpliedMode ;
    newCpu->table[0xBA].code = &TSX;
    newCpu->table[0x8A].mode = &ImpliedMode ;
    newCpu->table[0x8A].code = &TXA;
    newCpu->table[0x9A].mode = &ImpliedMode ;
    newCpu->table[0x9A].code = &TXS;
    newCpu->table[0x98].mode = &ImpliedMode ;
    newCpu->table[0x98].code = &TYA;
    
    newCpu->table[0x90].mode = &RelativeMode ; 
    newCpu->table[0x90].code = &BCC;
    newCpu->table[0xB0].mode = &RelativeMode ;
    newCpu->table[0xB0].code = &BCS;
    newCpu->table[0xF0].mode = &RelativeMode ;
    newCpu->table[0xF0].code = &BEQ;
    newCpu->table[0x30].mode = &RelativeMode ;
    newCpu->table[0x30].code = &BMI;
    newCpu->table[0xD0].mode = &RelativeMode ;
    newCpu->table[0xD0].code = &BNE;
    newCpu->table[0x10].mode = &RelativeMode ;
    newCpu->table[0x10].code = &BPL;
    newCpu->table[0x50].mode = &RelativeMode ;
    newCpu->table[0x50].code = &BVC;
    newCpu->table[0x70].mode = &RelativeMode ;
    newCpu->table[0x70].code = &BVS;

    return newCpu;

}
