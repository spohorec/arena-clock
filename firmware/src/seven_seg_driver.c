#include "seven_seg_driver.h"
#include "shift_register.h"

static shift_reg_bits_t seven_seg_off   = {0, 0, 0, 0, 0, 0, 0, 0};
static shift_reg_bits_t seven_seg_zero  = {1, 1, 1, 1, 1, 1, 0, 0};
static shift_reg_bits_t seven_seg_one   = {0, 1, 1, 0, 0, 0, 0, 0};
static shift_reg_bits_t seven_seg_two   = {1, 1, 0, 1, 1, 0, 1, 0};
static shift_reg_bits_t seven_seg_three = {1, 1, 1, 1, 0, 0, 1, 0};
static shift_reg_bits_t seven_seg_four  = {0, 1, 1, 0, 0, 1, 1, 0};
static shift_reg_bits_t seven_seg_five  = {1, 0, 1, 1, 0, 1, 1, 0};
static shift_reg_bits_t seven_seg_six   = {1, 0, 1, 1, 1, 1, 1, 0};
static shift_reg_bits_t seven_seg_seven = {1, 1, 1, 0, 0, 0, 0, 0};
static shift_reg_bits_t seven_seg_eight = {1, 1, 1, 1, 1, 1, 1, 0};
static shift_reg_bits_t seven_seg_nine  = {1, 1, 1, 1, 0, 1, 1, 0};

shift_reg_bits_t SevenSegment_SettingOff(void) { return seven_seg_off; }
shift_reg_bits_t SevenSegment_SettingZero(void) { return seven_seg_zero; }
shift_reg_bits_t SevenSegment_SettingOne(void) { return seven_seg_one; }
shift_reg_bits_t SevenSegment_SettingTwo(void) { return seven_seg_two; }
shift_reg_bits_t SevenSegment_SettingThree(void) { return seven_seg_three; }
shift_reg_bits_t SevenSegment_SettingFour(void) { return seven_seg_four; }
shift_reg_bits_t SevenSegment_SettingFive(void) { return seven_seg_five; }
shift_reg_bits_t SevenSegment_SettingSix(void) { return seven_seg_six; }
shift_reg_bits_t SevenSegment_SettingSeven(void) { return seven_seg_seven; }
shift_reg_bits_t SevenSegment_SettingEight(void) { return seven_seg_eight; }
shift_reg_bits_t SevenSegment_SettingNine(void) { return seven_seg_nine; }

shift_reg_bits_t SevenSegment_DigitToSetting(uint8_t digit)
{
    shift_reg_bits_t setting;
    switch (digit) {
        case 0:
            setting = SevenSegment_SettingZero();
            break;
        case 1:
            setting = SevenSegment_SettingOne();
            break;
        case 2:
            setting = SevenSegment_SettingTwo();
            break;
        case 3:
            setting = SevenSegment_SettingThree();
            break;
        case 4:
            setting = SevenSegment_SettingFour();
            break;
        case 5:
            setting = SevenSegment_SettingFive();
            break;
        case 6:
            setting = SevenSegment_SettingSix();
            break;
        case 7:
            setting = SevenSegment_SettingSeven();
            break;
        case 8:
            setting = SevenSegment_SettingEight();
            break;
        case 9:
            setting = SevenSegment_SettingNine();
            break;
        default:
            setting = SevenSegment_SettingOff();
            break;
    }
    return setting;
}
