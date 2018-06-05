import sys
sys.path.insert(0, 'Cubium/drivers/PythonDrivers')

import time
import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008

SPI_PORT = 0
SPI_DEVICE = 0
lightSensor = Adafruit_MCP3008.MCP3008(spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE))
LIGHTSENSOR1PIN = 2

#Lux conversions found here: https://arduinodiy.wordpress.com/2013/11/03/measuring-light-with-an-arduino/

def handleSpaData():
	pass

def sendData():
	lightSensor1 = lightSensor.read_adc(LIGHTSENSOR1PIN)

	values = lightSensor1
	values = values * .0048828125
	Lux = (2500.0/values-500.0)/10.0

	return Lux

def init():
	pass
