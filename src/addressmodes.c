#include <CPU6502/Internal/addressmodes.h>
#include <CPU6502/Internal/macros.h>

uint16_t ImpliedMode(mos6502* cpu) {

    return cpu->pc;

}

uint16_t AccumulatorMode(mos6502* cpu) {

    return cpu->pc;

}

uint16_t AbsoluteMode(mos6502* cpu) {

    uint16_t low_nibble = cpu->Read(cpu->pc++);
    uint16_t high_nibble = cpu->Read(cpu->pc++);

    return low_nibble | (high_nibble << 8);

}

uint16_t AbsoluteXMode(mos6502* cpu) {

    uint16_t low_nibble = cpu->Read(cpu->pc++);
    uint16_t high_nibble = cpu->Read(cpu->pc++);

    return (low_nibble | (high_nibble << 8)) + cpu->X + IS_SET(C);

}

uint16_t AbsoluteYMode(mos6502* cpu) {

    uint16_t low_nibble = cpu->Read(cpu->pc++);
    uint16_t high_nibble = cpu->Read(cpu->pc++);

    return (low_nibble | (high_nibble << 8)) + cpu->Y + IS_SET(C);

}

uint16_t ImmediateMode(mos6502* cpu) {

    return cpu->pc++;

}

uint16_t ZeroPageMode(mos6502* cpu) {

    return cpu->Read(cpu->pc++);

}

uint16_t ZeroPageXMode(mos6502* cpu) {

    return (cpu->Read(cpu->pc++) + cpu->X) & 0xFF;

}

uint16_t ZeroPageYMode(mos6502* cpu) {

    return (cpu->Read(cpu->pc++) + cpu->Y) & 0xFF;

}

uint16_t IndirectMode(mos6502* cpu) {

    uint16_t low_nibble = cpu->Read(cpu->pc++);
    uint16_t high_nibble = cpu->Read(cpu->pc++);
    uint16_t addr = (low_nibble) | (high_nibble << 8);
    uint16_t eff = cpu->Read(addr) | (cpu->Read((addr + 1) & 0xFF) << 8);

    return eff;

}

uint16_t IndirectXMode(mos6502* cpu) {

    uint16_t zlow_nib = (cpu->Read(cpu->pc++) + cpu->X) & 0xFF;
    uint16_t zhigh_nib = (zlow_nib + 1) & 0xFF;
    uint16_t addr = cpu->Read(zlow_nib) | (cpu->Read(zhigh_nib) << 8);

    return addr;
}

uint16_t IndirectYMode(mos6502* cpu) {

    uint16_t zlow_nib = (cpu->Read(cpu->pc++)) & 0xFF;
    uint16_t zhigh_nib = (zlow_nib + 1) & 0xFF;
    uint16_t addr = (cpu->Read(zlow_nib) | (cpu->Read(zhigh_nib) << 8)) + cpu->Y;

    return addr;
}

uint16_t RelativeMode(mos6502* cpu) {

    uint16_t offset = cpu->Read(cpu->pc++);
    offset |= offset & 0x80 ? 0xFF00 : 0;
    
    return cpu->pc + offset;

}
