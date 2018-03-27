print 'Initializing gyro...'

import time

from Adafruit_Python_BNO055 import BNO055

#bno = BNO055.BNO055(rst='P9_12', busnum=2)
bno = BNO055.BNO055(rst=3)

def handleSpaData():
    pass

def sendData():
    time.sleep(10)
    heading, roll, pitch = bno.read_euler()
    sys, gyro, accel, mag = bno.get_calibration_status()
    return 'H{0:0.2F},R{1:0.2F},P{2:0.2F},S{3},G{4},A{5},M{6}'.format(heading, roll, pitch, sys, gyro, accel, mag)
 
def init():
    if not bno.begin():
        raise RuntimeError('Failed to initialize BNO055! Is the sensor connected?')

    print "Initialization success!"
