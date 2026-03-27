import serial
import time

def to_decimal(data):
    """
    Преобразует байты в список десятичных чисел
    """
    return [b for b in data.rstrip(b'\n')]

ser = serial.Serial('COM14', baudrate=9600, timeout= None)

data0 = bytes([52,  0, 1, 0, 1, 30, 00, 1, 1])  #velocity
data1 = bytes([52,  0, 1, 0, 2, 10, 00, 1, 1])  # acs time
data3 = bytes([52,  0, 1, 0, 4, 00, 00, 1, 1])  # home x 
data4 = bytes([52,  0, 1, 0, 6, 00, 00, 1, 1])  # home y  
data5 = bytes([52,  0, 1, 0, 7,  0,  0, 1, 1])  # X pos
data6 = bytes([52,  0, 1, 0, 8, 80, 00, 1, 1])  # пщ 8000
data7 = bytes([52,  0, 4, 0, 4, 00, 00, 1, 1])  # read home x
data8 = bytes([52,  0, 4, 0, 6, 00, 00, 1, 1])  # read home y
data9 = bytes([52,  0, 1, 0, 3, 00, 00, 1, 1])  # stop
"""
data4 = bytes([52,  0, 1, 0, 8, 80, 00, 1, 1])
data5 = bytes([52,  0, 1, 0, 7, 1,   0, 1, 1])
data6 = bytes([52,  0, 4, 0, 1, 00, 00, 1, 1]) 
data7 = bytes([52,  0, 4, 0, 5, 00, 00, 1, 1]) 
"""

ser.write(data0)
line = ser.read_until(b'\n')
ser.reset_input_buffer()

ser.write(data1)
line = ser.read_until(b'\n')
ser.reset_input_buffer()


ser.write(data3)
line = ser.read_until(b'\n')
ser.reset_input_buffer()

ser.write(data4)
line = ser.read_until(b'\n')
ser.reset_input_buffer()


ser.write(data5)
line = ser.read_until(b'\n')
ser.reset_input_buffer()

ser.write(data6)
ser.write(data7)
line = ser.read_until(b'\n')
ser.reset_input_buffer()
print(to_decimal(line))
line = ser.read_until(b'\n')
ser.reset_input_buffer()
print(to_decimal(line))

#line = ser.read_until(b'\n')
#ser.reset_input_buffer()
#print(to_decimal(line))