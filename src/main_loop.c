#include "main.h"
#include "spi.h"

#include "seven_seg_driver.h"
#include "shift_register.h"

UART_HandleTypeDef extern huart2;
SPI_HandleTypeDef* reg_spi = &hspi1;
extern TIM_HandleTypeDef htim16;


static gpio_output_t digit_rclk = { .port = GPIOA, .pin = GPIO_PIN_8 };
static gpio_output_t aux_rclk = { .port = GPIOB, .pin = GPIO_PIN_0 };


volatile bool onesec_timer_flag = false;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    onesec_timer_flag = true;
    //GPIO_SetPin(aux_rclk, state);
    //state = !state;
}


void MainLoop(void)
{
    ShiftRegister_Init();
    HAL_TIM_Base_Start_IT(&htim16);

    uint8_t time_s = 100;

    for (;;) {
        if (onesec_timer_flag) {
            onesec_timer_flag = false;

            // set the display
            uint8_t digit_1 = time_s / 100;
            uint8_t digit_2 = (time_s - digit_1) / 10;
            uint8_t digit_3 = (time_s - digit_1*100 - digit_2*10);
            shift_reg_bits_t digit_1_setting = SevenSegment_DigitToSetting(digit_1);
            shift_reg_bits_t digit_2_setting = SevenSegment_DigitToSetting(digit_2);
            shift_reg_bits_t digit_3_setting = SevenSegment_DigitToSetting(digit_3);

            SetDigitShiftRegisters(digit_1_setting, digit_2_setting, digit_3_setting);

            time_s -= 1;

            //HAL_UART_Transmit(&huart2, buf, 8, 100);
        } else {
            HAL_Delay(1);
        }
    }
}
