import serial


scale = serial.Serial(
    port="/dev/cu.usbmodem142401",
    baudrate=9600,
    timeout= None
)

while 1:
    data = scale.readline().decode().strip()
    print(data)