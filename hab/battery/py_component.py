"""
Battery voltage reader
"""

import Adafruit_BBIO.ADC as ADC
ADC.setup()


def handleSpaData():

    pass

def sendData():
    analogInput = ADC.read("AIN0")
    analogInput = ADC.read("AIN0")
    
    voltage = float(analogInput * (5.0 / 1023.0))

    buffer = str(voltage)
    return buffer

def init():


