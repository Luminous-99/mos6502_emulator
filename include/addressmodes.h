#pragma once
#include <CPU6502/Internal/types.h>
#include <stdint.h>

uint16_t AccumulatorMode(mos6502* cpu);
uint16_t AbsoluteMode(mos6502* cpu);
uint16_t AbsoluteXMode(mos6502* cpu);
uint16_t AbsoluteYMode(mos6502* cpu);
uint16_t ImmediateMode(mos6502* cpu);
uint16_t ImpliedMode(mos6502* cpu);
uint16_t IndirectMode(mos6502* cpu);
uint16_t IndirectXMode(mos6502* cpu);
uint16_t IndirectYMode(mos6502* cpu);
uint16_t RelativeMode(mos6502* cpu);
uint16_t ZeroPageMode(mos6502* cpu);
uint16_t ZeroPageXMode(mos6502* cpu);
uint16_t ZeroPageYMode(mos6502* cpu);
