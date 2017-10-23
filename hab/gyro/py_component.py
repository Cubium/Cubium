import logging
import sys
import time

from Adafruit_BNO055 import BNO055

bno = BNO055.BNO055(rst='P9_12')

def handleSpaData():
    pass

def sendData():
    time.sleep(1)
    return "Pythonic Gyro Data!"
#    heading, roll, pitch = bno.read_euler()
#    return '{0:0.2F},{1:0.2F},{2:0.2F}'.format(heading, roll, pitch, sys, gyro, accel, mag)
 

def init():
#    if not bno.begin():
#        raise RuntimeError('Failed to initialize BNO055! Is the sensor connected?')

    sys.stdout.write("python component beginning initialization...\n")

