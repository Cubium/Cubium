"""
Battery voltage reader
"""

import Adafruit_BBIO.ADC as ADC
import time
ADC.setup()


def handleSpaData():

    pass

def sendData():
    analogInput = ADC.read("AIN0")
    analogInput = ADC.read("AIN0")
    
    voltage = float(analogInput * (5.0 / 1023.0))
   # print analogInput

    buffer = str(voltage)
    return buffer

def init():

    pass


#test output data
# while True:
    #
    # print sendData()
    #
    # time.sleep(1)
    #


