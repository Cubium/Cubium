import sys
sys.path.insert(0, 'Cubium/drivers/PythonDrivers')

import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008

import time

SPI_PORT = 0
SPI_DEVICE = 0
uv_external = Adafruit_MCP3008.MCP3008(spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE))
ADC_PIN = 1

def handleSpaData():
    pass

def sendData():
    return (float(uv_external.read_adc(ADC_PIN)))

def init():
    pass

while True:
    print sendData()
    time.sleep(1)
