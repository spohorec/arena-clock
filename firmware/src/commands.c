
#include "commands.h"
#include "timer.h"

/*
RX is one byte for type, one byte for payload (can be any byte if not used).
Currently only used for set time.

Reponse is one byte type, mirroring the recieved type plus one byte, zero or one depending on command success.
*/


typedef enum {
    MSG_TYPE_UNKNOWN = 0,
    MSG_TYPE_HELLO,
    MSG_TYPE_SET_TIME,
    MSG_TYPE_START_TIMER,
    MSG_TYPE_PAUSE_TIMER,
    MSG_TYPE_CANCEL_TIMER,
    MSG_TYPE_TIMER_DONE,
} msg_type_t;

int ProcessUnknownCommand(void) {
    return 0;
}

int ProcessHelloCommand(void)
{
    return 1;
}

int ProcessSetTimeCommand(uint8_t n_secs)
{
    if (n_secs > 599) { // Can only go up to 9:59
        return 0;
    } else if (Timer_GetState() != TIMER_STATE_IDLE) {
        return 0;
    } else {
        Timer_SetTime(n_secs);
        return 1;
    }
}

int ProcessStartTimerCommand(void)
{
    if (Timer_GetState() == TIMER_STATE_RUNNING) {
        return 0;
    } else {
        Timer_Start();
        return 1;
    }
}

int ProcessPauseTimerCommand(void)
{
    if (Timer_GetState() != TIMER_STATE_RUNNING) {
        return 0;
    } else {
        Timer_Pause();
        return 1;
    }
}

int ProcessCancelTimerCommand(void)
{
    if (Timer_GetState() == TIMER_STATE_RUNNING) {
        return 0;
    } else {
        Timer_Clear();
        return 1;
    }
}

void ProcessCommand(uint8_t* rx_buf, uint8_t* tx_buf)
{
    msg_type_t msg_type = rx_buf[0];
    tx_buf[0] = msg_type;
    switch (msg_type) {
        case MSG_TYPE_HELLO:
            tx_buf[1] = ProcessHelloCommand();
            break;
        case MSG_TYPE_SET_TIME:
            tx_buf[1] = ProcessSetTimeCommand(rx_buf[1]);
            break;
        case MSG_TYPE_START_TIMER:
            tx_buf[1] = ProcessStartTimerCommand();
            break;
        case MSG_TYPE_PAUSE_TIMER:
            tx_buf[1] = ProcessPauseTimerCommand();
            break;
        case MSG_TYPE_CANCEL_TIMER:
            tx_buf[1] = ProcessCancelTimerCommand();
            break;
        default:
            tx_buf[1] = ProcessUnknownCommand();
            break;
    }
}

void PopulateTimerCompleteNotification(uint8_t * tx_buf)
{
    tx_buf[0] = MSG_TYPE_TIMER_DONE;
    tx_buf[1] = 1;
}
