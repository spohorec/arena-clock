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

class Timer(QObject):
    _time_set_signal = pyqtSignal()
    _time_start_signal = pyqtSignal()
    _time_pause_signal = pyqtSignal()
    def __init__(self, port):
        super().__init__()
        self._running = False
        self._time_set = False
        self._ser = serial.Serial(port, 9600, timeout=1)

        self._worker = SerialWorker(self._ser)
        self._timer_done_signal = self._worker.timer_done
        self._timer_done_signal.connect(self.TimerDone)


    def __del__(self):
        self._ser.close()
        
    def GetTimerDoneSignal(self):
        return self._timer_done_signal
    def IsRunning(self):
        return self._running;
    def IsTimeSet(self):
        return self._time_set
    def TimerDone(self):
        self._running = False
        self._time_set = False
    def SetTime(self, time_s):
        if (time_s > 255):
            return
        #if self.IsRunning():
        #    return
        self._ser.write(bytearray([2, time_s]))
        rep = self._ser.read(2)
        if (rep[0] != 2 or rep[1] !=1):
            return
        self._time_set = True;
        self._time_set_signal.emit()
    def Start(self):
        self._ser.write(bytearray([3, 0]))
        rep = self._ser.read(2)
        if (rep[0] != 3 or rep[1] !=1):
            return
        self._running = True
        self._worker.start()
        self._time_start_signal.emit()
    def Pause(self):
        self._worker.stop()
        while (not self._worker._stopped):
            continue;
        self._ser.write(bytearray([4, 0]))
        rep = self._ser.read(2)
        if (rep[0] != 4 or rep[1] !=1):
            return
        self._running = False
        self._time_pause_signal.emit()
    def Clear(self):
        self._ser.write(bytearray([5, 0]))
        rep = self._ser.read(2)
        if (rep[0] != 5 or rep[1] !=1):
            print("bad response from clear!")
            return
        self._running = False
        self._time_set = False
        self._timer_done_signal.emit()
    def Finish(self):
        print("hello")
        return


port = sys.argv[1]
timer = Timer(port)

app = QApplication([])
window = QWidget()


start_pause_button = QPushButton('Start')
start_pause_button.setDisabled(True)
clear_button = QPushButton('Clear')
set_time_button = QPushButton('Set Time')

timer_widget = QTimeEdit()

def on_start_pause_button_click():
    #btn = QApplication.focusWidget()
    if timer.IsRunning():
        timer.Pause()
        #start_pause_button.setText('Start')
        #clear_button.setDisabled(False)
    else:
        # timer not running
        timer.Start()
        #start_pause_button.setText('Pause')
        #clear_button.setDisabled(True)
        

def on_clear_button_click():
    timer.Clear()

def on_timer_start():
    # gray out clear
    clear_button.setDisabled(True)
    # start > pause
    start_pause_button.setText('Pause')
    # gray out time set
    set_time_button.setDisabled(True);

def on_timer_pause():
    # active clear
    clear_button.setDisabled(False)
    # pause > start
    start_pause_button.setText('Start')
    # gray out time set
    set_time_button.setDisabled(True);

def on_timer_finish():
    # disable start button
    start_pause_button.setText('Start')
    start_pause_button.setDisabled(True)
    # active clear
    clear_button.setDisabled(False)
    # set time button disable
    set_time_button.setDisabled(False);
    timer_widget.setDisabled(False)

def on_timer_time_set():
    # activate start button
    start_pause_button.setText('Start')
    start_pause_button.setDisabled(False)
    timer_widget.setDisabled(True)


timer.GetTimerDoneSignal().connect(on_timer_finish)
timer._time_start_signal.connect(on_timer_start)
timer._time_pause_signal.connect(on_timer_pause)
timer._time_set_signal.connect(on_timer_time_set)

outer_layout = QVBoxLayout()
hlayout1 = QHBoxLayout()
hlayout2 = QHBoxLayout()

# Timer display
timer_widget.setDisplayFormat("mm:ss")

def on_set_time_button_click():
    time = timer_widget.time()
    time_s = time.minute()*60+time.second()
    timer.SetTime(time_s)


set_time_button.clicked.connect(on_set_time_button_click)

hlayout1.addWidget(timer_widget)
hlayout1.addWidget(set_time_button)

# Buttons
start_pause_button.clicked.connect(on_start_pause_button_click)

clear_button.clicked.connect(on_clear_button_click)

def ResetOnTimerDone():
    clear_button.setText('Start')

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

