#ifndef ALARM_CLOCK__GPIO_H
#define ALARM_CLOCK__GPIO_H

#include <stdbool.h>

#include "gpio.h"

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} gpio_output_t;

inline void GPIO_SetPin(gpio_output_t gpio, bool level)
{
    HAL_GPIO_WritePin(gpio.port, gpio.pin, level ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

#endif // ALARM_CLOCK__GPIO_H
