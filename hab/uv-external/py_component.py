import Adafruit_ADS1x15
import time

uv_external = Adafruit_ADS1x15.ADS1115()
GAIN = 1
ADC_PIN = 0

def handleSpaData():
    pass

def sendData():
    time.sleep(1)
    value = uv_external.read_adc(ADC_PIN, gain=GAIN)
    print value
    return value

def init():
    pass
