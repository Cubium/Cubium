print 'Initializing gyro...'


#from Adafruit_Python_BNO055 import BNO055

#the RST Pin on the Gyroscope.
#bno = BNO055.BNO055(rst=18)

def handleSpaData():
    pass

i = 0.0

def sendData():
    global i
    i += 1.0
    return "gyro data" + str(i)
#    heading, roll, pitch = bno.read_euler()
#    sys, gyro, accel, mag = bno.get_calibration_status()
    #print ('H{0:0.2F},R{1:0.2F},P{2:0.2F},S{3},G{4},A{5},M{6}'.format(heading, roll, pitch, sys, gyro, accel, mag))
#    return 'H{0:0.2F},R{1:0.2F},P{2:0.2F},S{3},G{4},A{5},M{6}'.format(heading, roll, pitch, sys, gyro, accel, mag)
 
def init():
#    if not bno.begin():
#        raise RuntimeError('Failed to initialize BNO055! Is the sensor connected?')

    print "Initialization success!"
