from PyQt5.QtWidgets import *
from PyQt5.QtCore import QThread, QObject, pyqtSignal

import serial
import sys

# Thread worker to monitor the serial port for the timer done notification
class SerialWorker(QThread):
    timer_done = pyqtSignal()

    def __init__(self, serial_conn):
        super().__init__()
        self._serial_conn = serial_conn # Pass the serial connection to avoid opening a second one.
        self._stop_flag = False; # signals to the thread to stop monitoring the port, leaving it open for other transactions
        self._stopped = True; # used by another process to ensure that the thread has stopped before continuing.


    def run(self):
        # wait for timer finished notification from the nucleo
        self._stopped = False;
        self._stop_flag = False;
        while not self._stop_flag:
            rep = self._serial_conn.read(2)
            if (rep):
                self.timer_done.emit()
                break;
        self._stopped = True;

    def stop(self):
        self._stop_flag = True

# Timer class to write commands to the nucleo
class Timer(QObject):
    _time_set_signal = pyqtSignal()
    _time_start_signal = pyqtSignal()
    _time_pause_signal = pyqtSignal()

    STATE_IDLE = 0
    STATE_RUNNING = 1
    STATE_PAUSED = 2
    def __init__(self, port):
        super().__init__()
        self._state = self.STATE_IDLE
        self._time_set = False
        self._ser = serial.Serial(port, 9600, timeout=1)

        self._worker = SerialWorker(self._ser)
        self._timer_done_signal = self._worker.timer_done
        self._timer_done_signal.connect(self.TimerDone)


    def __del__(self):
        self._ser.close() # close serial port on destruction
        
    # Signal getters
    def GetTimerDoneSignal(self):
        return self._timer_done_signal
    def GetTimerStartSignal(self):
        return self._timer_start_signal
    def GetTimerPauseSignal(self):
        return self._timer_pause_signal
    def GetTimeSetSignal(self):
        return self._time_set_signal

    # Status getters
    def IsRunning(self):
        return self._state == self.STATE_RUNNING
    def IsTimeSet(self):
        return self._time_set

    # Timer done handler
    def TimerDone(self):
        self._state = self.STATE_IDLE
        self._time_set = False

    # Serial command helper
    def SerialCommand(command_num : int, argument : int = 0):
        if command_num > 0xFF or argument > 0xFFFF:
            return False
        self._ser.write(byte_array([command_num, (argument & 0xFF00) >> 8 , argument & 0x00FF])
        rep = self._ser.read(2)
        if (rep[0] != command_num or rep[1] != 1):
            return False
        return True

    #Handle timer actions
    def SetTime(self, time_s):
        if (time_s > 60*9+59) || (time_s < 0):
            return
        if self._state != self.STATE_IDLE
            # Can only set time when system is idle
            return
        if not self.SerialCommand(2, time_s):
            return
        self._time_set = True;
        self._time_set_signal.emit()

    def Start(self):
        if self._state == self.STATE_RUNNING:
            return
        if not self.SerialCommand(3):
            return
        self._state = self.STATE_RUNNING
        self._worker.start() # start worker to monitor for timer done notification
        self._time_start_signal.emit()

    def Pause(self):
        if self._state != self.STATE_RUNNING:
            return
        self._worker.stop()
        while not self._worker._stopped:
            # wait for worker to stop before using the serial port
            continue;
        if not self.SerialCommand(4):
            return
        self._state = self.STATE_PAUSED
        self._time_pause_signal.emit()

    def Clear(self):
        if self._state == self.STATE_RUNNING:
            return
        if not self.SerialCommand(5):
            return
        self._state = self.STATE_IDLE
        self._time_set = False
        self._timer_done_signal.emit()

# APP =========================================================================

# Get serial port from arguments
port = sys.argv[1]
timer = Timer(port)

# Create app and window
app = QApplication([])
window = QWidget()

# Create widgets
start_pause_button = QPushButton('Start')
clear_button = QPushButton('Clear')
set_time_button = QPushButton('Set Time')
timer_widget = QTimeEdit()

# Setup for widgets
timer_widget.setDisplayFormat("mm:ss")
start_pause_button.setDisabled(True)

# Button handlers
def on_start_pause_button_click():
    if timer.IsRunning():
        timer.Pause()
    else:
        # timer not running
        timer.Start()

def on_clear_button_click():
    timer.Clear()

def on_set_time_button_click():
    time = timer_widget.time()
    time_s = time.minute()*60+time.second() # convert to seconds
    timer.SetTime(time_s)

# Button connections
start_pause_button.clicked.connect(on_start_pause_button_click)
clear_button.clicked.connect(on_clear_button_click)
set_time_button.clicked.connect(on_set_time_button_click)

# Widget state updaters, based on timer actions
def on_timer_start():
    # disable clear
    clear_button.setDisabled(True)
    # display "pause"
    # enable start/pause button
    start_pause_button.setText('Pause')
    start_pause_button.setDisabled(False)
    # disable time set
    set_time_button.setDisabled(True);
    timer_widget.setDisabled(True)

def on_timer_pause():
    # enable clear
    clear_button.setDisabled(False)
    # display "start"
    # enable start/pause button
    start_pause_button.setText('Start')
    start_pause_button.setDisabled(False)
    # disable time set
    set_time_button.setDisabled(True);
    timer_widget.setDisabled(True)

def on_timer_finish():
    # display "start"
    # disable start/pause button
    start_pause_button.setText('Start')
    start_pause_button.setDisabled(True)
    # enable clear
    clear_button.setDisabled(False)
    # enable time set
    set_time_button.setDisabled(False);
    timer_widget.setDisabled(False)

def on_timer_time_set():
    # display "start"
    # enable start button
    start_pause_button.setText('Start')
    start_pause_button.setDisabled(False)
    # enable clear
    clear_button.setDisabled(False)
    # enable time set
    set_time_button.setDisabled(False);
    timer_widget.setDisabled(False)


# Connect timer signals to handlers
timer.GetTimerDoneSignal().connect(on_timer_finish)
timer.GetTimerStartSignal().connect(on_timer_start)
timer.GetTimerPauseSignal().connect(on_timer_pause)
timer.GetTimeSetSignal().connect(on_timer_time_set)

# Layouts
outer_layout = QVBoxLayout()
hlayout1 = QHBoxLayout()
hlayout2 = QHBoxLayout()

hlayout1.addWidget(timer_widget)
hlayout1.addWidget(set_time_button)

hlayout2.addWidget(start_pause_button)
hlayout2.addWidget(clear_button)

# Add row layouts
outer_layout.addLayout(hlayout1)
outer_layout.addLayout(hlayout2)

# Outer  layout
window.setLayout(outer_layout)
window.show()
window.setWindowTitle("Arena Clock")
window.setMinimumSize(300, 100)
app.exec()

