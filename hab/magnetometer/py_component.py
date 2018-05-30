import Adafruit_LSM303

lsm = Adafruit_LSM303.LSM303()

def handleSpaData():
    pass

def sendData():
    accel, mag = lsm.read()
    accel_x, accel_y, accel_z = accel
    mag_x,mag_y,mag_z = mag

    return 'Accel: X={}, Y={}, Z={}; Mag: X={}, Y={}, Z={}'.format(accel_x,accel_y,accel_z,mag_x,mag_y,mag_z)
 
def init():
    print "Initialization success!"
