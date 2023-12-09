#ifndef ARENA_CLOCK__TIMER_H
#define ARENA_CLOCK__TIMER_H

#include <stdint.h>

typedef enum {
    TIMER_STATE_IDLE = 0,
    TIMER_STATE_RUNNING,
    TIMER_STATE_PAUSED,
} timer_state_t;

void Timer_OneSecCallback(void);
timer_state_t Timer_GetState(void);

void Timer_SetTime(uint16_t n_secs);
void Timer_Start(void);
void Timer_Pause(void);
void Timer_Clear(void);


#endif // ARENA_CLOCK__TIMER_H
