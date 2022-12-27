import serial
import serial.tools.list_ports as list_ports
from threading import main_thread

buttons = {'U': 0, 'D': 0, 'L': 0, 'R': 0, 'P': 0}
zero_count = {'U': 0, 'D': 0, 'L': 0, 'R': 0, 'P': 0}
ESCAPE = False

def get_default_arduino():
    ports = list(list_ports.comports())
    arduino_found = False
    for port in ports:
        if 'ACM' in port.description or 'Arduino' in port.description or 'USB Serial' in port.description:
            arduino = serial.Serial(port.device, baudrate=115200, timeout=.1)
            print('Arduino in port {} connected'.format(port.device))
            arduino_found = True
            return arduino
    if not arduino_found:
        print('Arduino not found')
        return None

def collect_arduino_data(arduino = None):
    if not arduino:
        arduino = get_default_arduino()
        if not arduino: return
    while True:
        temp = arduino.readline()
        if temp != b'' and temp != b'\r\n':
            temp = temp.decode()
            key = temp[0]
            value = int(temp[1:-2])
            if value == 0:
                zero_count[key] += 1
                if zero_count[key] > 5:
                    buttons[key] = 0
                    zero_count[key] = 0
            else:
                buttons[key] = value
        if not main_thread().is_alive(): return
