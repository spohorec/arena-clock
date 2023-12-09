#include "main.h"
#include "spi.h"

#include "seven_seg_driver.h"
#include "shift_register.h"
#include "commands.h"
#include "timer.h"


uint8_t uart_rx_buffer[kCommandBufferRXLen];
uint8_t uart_tx_buffer[kCommandBufferTXLen];

/* HAL Objects */
UART_HandleTypeDef extern huart2;
SPI_HandleTypeDef* reg_spi = &hspi1;
extern TIM_HandleTypeDef htim16;

// GPIO Definitions
static gpio_output_t digit_rclk = { .port = GPIOA, .pin = GPIO_PIN_8 };
static gpio_output_t aux_rclk = { .port = GPIOB, .pin = GPIO_PIN_0 };
static gpio_output_t nuc_led = { .port = GPIOB, .pin = GPIO_PIN_3 };

/* Flags for callbacks*/
volatile bool onesec_timer_flag = false;
volatile bool uart_rx_flag = false;


/* Callbacks */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    onesec_timer_flag = true;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uart_rx_flag = true;
}

void NotifyTimerComplete()
{
    PopulateTimerCompleteNotification(uart_tx_buffer);
    HAL_UART_Transmit(&huart2, uart_tx_buffer, kCommandBufferTXLen, 100);
}


static bool led_state = false;
void MainLoop(void)
{
    ShiftRegister_Init();
    Timer_Init(&htim16);
    HAL_UART_Receive_IT(&huart2, uart_rx_buffer, kCommandBufferRXLen);
    GPIO_SetPin(nuc_led, led_state);

    for (;;) {
        // Handle UART
        if (uart_rx_flag) {
            uart_rx_flag = false;
            ProcessCommand(uart_rx_buffer, uart_tx_buffer);
            HAL_UART_Transmit(&huart2, uart_tx_buffer, kCommandBufferTXLen, 100);
            HAL_UART_Receive_IT(&huart2, uart_rx_buffer, kCommandBufferRXLen);
        }

        // Handle one second timer
        if (onesec_timer_flag) {
            GPIO_SetPin(nuc_led, led_state);
            led_state = !led_state;
            onesec_timer_flag = false;
            Timer_OneSecCallback();
        }

        // Wait a little bit
        HAL_Delay(1);
    }
}

/*

            // set the display
            uint8_t digit_1 = time_s / 100;
            uint8_t digit_2 = (time_s - digit_1) / 10;
            uint8_t digit_3 = (time_s - digit_1*100 - digit_2*10);
            shift_reg_bits_t digit_1_setting = SevenSegment_DigitToSetting(digit_1);
            shift_reg_bits_t digit_2_setting = SevenSegment_DigitToSetting(digit_2);
            shift_reg_bits_t digit_3_setting = SevenSegment_DigitToSetting(digit_3);

            SetDigitShiftRegisters(digit_1_setting, digit_2_setting, digit_3_setting);

            time_s -= 1;

*/
