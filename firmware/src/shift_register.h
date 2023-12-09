#ifndef ARENA_CLOCK__SHIFT_REGISTER_H
#define ARENA_CLOCK__SHIFT_REGISTER_H

#include "gpio_drv.h"

#define kShiftRegNBits 8

typedef struct {
    bool b1;
    bool b2;
    bool b3;
    bool b4;
    bool b5;
    bool b6;
    bool b7;
    bool b8;
} shift_reg_bits_t;

void ShiftRegister_Init();
void ShiftRegister_ClearAll();
void ShiftRegister_EnableAll();
void ShiftRegister_DisableAll();
bool ShiftRegister_SendBits(shift_reg_bits_t bits);
bool SetDigitShiftRegisters(shift_reg_bits_t digit1, shift_reg_bits_t digit2, shift_reg_bits_t digit3);

#endif // ARENA_CLOCK__SHIFT_REGISTER_H
