#ifndef ARENA_CLOCK__SEVEN_SEG_DRIVER_H
#define ARENA_CLOCK__SEVEN_SEG_DRIVER_H

#include "shift_register.h"

shift_reg_bits_t SevenSegment_SettingOff(void);
shift_reg_bits_t SevenSegment_SettingZero(void);
shift_reg_bits_t SevenSegment_SettingOne(void);
shift_reg_bits_t SevenSegment_SettingTwo(void);
shift_reg_bits_t SevenSegment_SettingThree(void);
shift_reg_bits_t SevenSegment_SettingFour(void);
shift_reg_bits_t SevenSegment_SettingFive(void);
shift_reg_bits_t SevenSegment_SettingSix(void);
shift_reg_bits_t SevenSegment_SettingSeven(void);
shift_reg_bits_t SevenSegment_SettingEight(void);
shift_reg_bits_t SevenSegment_SettingNine(void);

shift_reg_bits_t SevenSegment_DigitToSetting(uint8_t digit);

#endif // ARENA_CLOCK__SEVEN_SEG_DRIVER_H
