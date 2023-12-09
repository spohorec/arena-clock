# GUI To control clock

## Requirements

* Python 3.6 +
* Modules: PyQt5, sys, serial

## How to use:

Plug USB into nucleo and computer and run
`python3.6 gui.py [insert serial port here (COMX or /dev/ttyXYZ)]`

Type desired time (min:sec) into time entry. Click "set time" to send to the nucleo. The "clear" button zeroes the time on the clock. The start/pause button controls when the clock counts down. Once the timer has been started, it must count to zero or be cleared before a new time can be entered.
