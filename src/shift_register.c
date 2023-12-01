#include "shift_register.h"

#include "spi.h"

static gpio_output_t n_clr = { .port = GPIOB, .pin = GPIO_PIN_7 };
static gpio_output_t n_oe = { .port = GPIOA, .pin = GPIO_PIN_12 };

static gpio_output_t digit_rclk = { .port = GPIOA, .pin = GPIO_PIN_8 };
static gpio_output_t aux_rclk = { .port = GPIOB, .pin = GPIO_PIN_0 };

SPI_HandleTypeDef* shift_reg_spi = &hspi1;


void ShiftRegister_Init()
{
    ShiftRegister_ClearAll();
    ShiftRegister_EnableAll();
}

void ShiftRegister_ClearAll()
{
   GPIO_SetPin(n_clr, 0);
   HAL_Delay(1);
   GPIO_SetPin(n_clr, 1);
}

void ShiftRegister_EnableAll()
{
    GPIO_SetPin(n_oe, 0);
}

void ShiftRegister_DisableAll()
{
    GPIO_SetPin(n_oe, 1);
}

bool ShiftRegister_SendBits(shift_reg_bits_t bits)
{

    uint8_t data =
           bits.b1 << 7 |
           bits.b2 << 6 |
           bits.b3 << 5 |
           bits.b4 << 4 |
           bits.b5 << 3 |
           bits.b6 << 2 |
           bits.b7 << 1 |
           bits.b8 << 0;

/*
    uint8_t data[kShiftRegNBits] = {
        bits.b1,
        bits.b2, 
        bits.b3, 
        bits.b4, 
        bits.b5, 
        bits.b6, 
        bits.b7, 
        bits.b8};
*/
    HAL_StatusTypeDef res = HAL_SPI_Transmit(shift_reg_spi, &data, 1, 1000);
    return (res == HAL_OK);
}

void ShiftRegister_PulseRegClock(gpio_output_t clk)
{
    GPIO_SetPin(clk, 1);
    HAL_Delay(2);
    GPIO_SetPin(clk, 0);
}

bool SetDigitShiftRegisters(shift_reg_bits_t digit1, shift_reg_bits_t digit2, shift_reg_bits_t digit3)
{
    // Send the three digit bits in reverse order since they shift 1->2->3
    ShiftRegister_SendBits(digit3);
    ShiftRegister_SendBits(digit2);
    ShiftRegister_SendBits(digit1);

    ShiftRegister_PulseRegClock(digit_rclk);

    return true;
}
