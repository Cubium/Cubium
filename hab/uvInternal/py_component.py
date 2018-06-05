import sys
sys.path.insert(0, 'Cubium/drivers/PythonDrivers')

import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008

import time

SPI_PORT = 0
SPI_DEVICE = 0
uv_internal = Adafruit_MCP3008.MCP3008(spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE))
ADC_PIN = 0

def handleSpaData():
    pass

def sendData():
    value = uv_internal.read_adc(ADC_PIN)
    return value

def init():
    pass
