import serial

ser = serial.Serial('COM14', baudrate=9600, timeout=1)

data = bytes([1, 2, 3, 4, 5, 6, 7, 8, 9])  # или bytes([123, 456789]) - но 456789 > 255
ser.write(data)