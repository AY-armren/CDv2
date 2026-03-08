import serial
import time
ser = serial.Serial('COM14', baudrate=9600, timeout= None)

data0 = bytes([52,  0, 1, 0, 1, 30, 00, 1, 1])  # или bytes([123, 456789]) - но 456789 > 255
data1 = bytes([52,  0, 1, 0, 2, 10, 00, 1, 1])  # или bytes([123, 456789]) - но 456789 > 255
data2 = bytes([52,  0, 1, 0, 3, 50, 00, 1, 1])  # или bytes([123, 456789]) - но 456789 > 255
data3 = bytes([52,  0, 1, 0, 4, 00, 00, 1, 1])
data4 = bytes([52,  0, 1, 0, 8, 50, 00, 1, 1])
ser.write(data0)
line = ser.read_until(b'\n')
ser.write(data1)
line = ser.read_until(b'\n')
ser.write(data2)
line = ser.read_until(b'\n')
ser.write(data3)
line = ser.read_until(b'\n')
ser.write(data2)
line = ser.read_until(b'\n')
ser.write(data4)
line = ser.read_until(b'\n')