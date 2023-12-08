#include "timer.h"

#include <stdint.h>

#include "main_loop.h"
#include "seven_seg_driver.h"


// Timer state and time
static uint8_t timer_remaining_time;
static timer_state_t timer_state;
static TIM_HandleTypeDef* htim;  

/* Local Functions */
static void PutTimeOnDigits(uint8_t time_s) {
    uint8_t digit_1, digit_2, digit_3;

    // Calc minutes and seconds from seconds
    uint8_t n_minutes = time_s / 60;
    uint8_t n_secs = time_s - (n_minutes * 60);

    // Calculate each place value
    digit_1 = n_minutes;
    digit_2 = n_secs / 10;
    digit_3 = n_secs % 10;

    // Convert to shift register settings
    shift_reg_bits_t digit_1_setting = SevenSegment_DigitToSetting(digit_1);
    shift_reg_bits_t digit_2_setting = SevenSegment_DigitToSetting(digit_2);
    shift_reg_bits_t digit_3_setting = SevenSegment_DigitToSetting(digit_3);

    // Set shift register
    SetDigitShiftRegisters(digit_1_setting, digit_2_setting, digit_3_setting);
}

/* API */
void Timer_Init(TIM_HandleTypeDef *hal_htim)
{
    htim = hal_htim;
    timer_remaining_time = 0;
    timer_state = TIMER_STATE_IDLE;
    SetDigitShiftRegisters(SevenSegment_SettingOff(), SevenSegment_SettingOff(), SevenSegment_SettingOff());
}

timer_state_t Timer_GetState(void)
{
    return timer_state;
}

void Timer_SetTime(uint8_t n_secs)
{
    if (timer_state == TIMER_STATE_IDLE) {
        timer_remaining_time = n_secs;
        PutTimeOnDigits(timer_remaining_time);
    }
}

void Timer_Start(void)
{
    if (timer_state != TIMER_STATE_RUNNING) {
        timer_state = TIMER_STATE_RUNNING;
        HAL_TIM_Base_Start_IT(htim);
    }
}

void Timer_Pause(void)
{
    if (timer_state == TIMER_STATE_RUNNING) {
        HAL_TIM_Base_Stop_IT(htim);
        timer_state = TIMER_STATE_PAUSED;
    }
}

void Timer_Clear(void)
{
    if (timer_state != TIMER_STATE_IDLE) {
        HAL_TIM_Base_Stop_IT(htim);
        timer_state = TIMER_STATE_IDLE;
    }

    timer_remaining_time = 0;
    PutTimeOnDigits(timer_remaining_time);
}

void Timer_OneSecCallback(void)
{
    if (timer_state == TIMER_STATE_RUNNING) {
        // decrement time remaining
        timer_remaining_time -= 1;
        // set digits
        PutTimeOnDigits(timer_remaining_time);
        if (timer_remaining_time == 0) {
            Timer_Clear();
            NotifyTimerComplete();
        }
    }
}
