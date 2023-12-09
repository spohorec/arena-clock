#ifndef ARENA_CLOCK__COMMANDS_H
#define ARENA_CLOCK__COMMANDS_H

#include <stdint.h>

#define kCommandBufferTXLen 2
#define kCommandBufferRXLen 3

void ProcessCommand(uint8_t *rx_buf, uint8_t *tx_buf);
void PopulateTimerCompleteNotification(uint8_t * tx_buf);

#endif // ARENA_CLOCK__COMMANDS_H
