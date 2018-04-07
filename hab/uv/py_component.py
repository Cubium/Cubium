from Adafruit_ADS1x15.ADS1x15 import *
import time

uv_sensor = Adafruit_ADS1x15.ADS1115()
GAIN = 1
def handleSpaData():
    pass

def sendData():
    time.sleep(1)
    value = uv_sensor.read_adc(3, gain=GAIN)
    print value
    return sensor.readTempC()

def init():
    pass
