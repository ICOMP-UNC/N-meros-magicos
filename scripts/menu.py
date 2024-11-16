import serial
import threading
import time

s =  serial.Serial("/dev/ttyUSB0",
                   timeout=0.1)

def work():
    while True:
        b = s.readline()
        if len(b):
            print(b.decode())

thread = threading.Thread(target=work)
thread.start()
while True:
    d = input("cmd: ")
    s.write(d.encode())
