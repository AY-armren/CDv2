import serial
import time

def to_decimal(data):
    """
    Преобразует байты в список десятичных чисел
    """
    return [b for b in data.rstrip(b'\n')]

ser = serial.Serial('COM14', baudrate=9600, timeout= None)

data0 = bytes([52,  0, 1, 0, 1, 30, 00, 1, 1])  # или bytes([123, 456789]) - но 456789 > 255
data1 = bytes([52,  0, 1, 0, 2, 2, 00, 1, 1])  # или bytes([123, 456789]) - но 456789 > 255
data2 = bytes([52,  0, 1, 0, 3, 50, 00, 1, 1])  # или bytes([123, 456789]) - но 456789 > 255
data3 = bytes([52,  0, 1, 0, 4, 00, 00, 1, 1])
data4 = bytes([52,  0, 1, 0, 8, 50, 00, 1, 1])
data5 = bytes([52,  0, 1, 0, 7, 0, 1, 1, 1])
data6 = bytes([52,  0, 4, 0, 1, 00, 00, 1, 1]) 
data7 = bytes([52,  0, 4, 0, 5, 00, 00, 1, 1]) 

ser.write(data0)
line = ser.read_until(b'\n')
ser.reset_input_buffer()

ser.write(data1)
line = ser.read_until(b'\n')
ser.reset_input_buffer()

ser.write(data2)
line = ser.read_until(b'\n')
ser.reset_input_buffer()

ser.write(data3)
line = ser.read_until(b'\n')
ser.reset_input_buffer()

ser.write(data2)
line = ser.read_until(b'\n')
ser.reset_input_buffer()
#ось 
ser.write(data5)
line = ser.read_until(b'\n')
ser.reset_input_buffer()
#поехали
ser.write(data4)
line = ser.read_until(b'\n')
ser.reset_input_buffer()
#читаем
ser.write(data6)
line = ser.read_until(b'\n')
ser.reset_input_buffer()

print(to_decimal(line))

#читаем
ser.write(data7)
line = ser.read_until(b'\n')
ser.reset_input_buffer()

print(to_decimal(line))