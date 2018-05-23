"""
import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008

SPI_PORT = 0
SPI_DEVICE = 0
lightSensor = Adafruit_MCP3008.MCP3008(spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE))
LIGHTSENSOR1PIN = 2
LIGHTSENSOR2PIN = 3

#Lux conversions found here: https://arduinodiy.wordpress.com/2013/11/03/measuring-light-with-an-arduino/
"""

i = 0.0

def handleSpaData():
	pass

def sendData():
    global i
    i += 1
    return 42.0 + i
    """
	lightSensor1 = lightSensor.read_adc(LIGHTSENSOR1PIN)
	lightSensor2 = lightSensor.read_adc(LIGHTSENSOR2PIN)

	values = (lightSensor1 + lightSensor2)/2
	values = values * .0048828125
	Lux = (2500.0/values-500.0)/10.0

	time.sleep(1)

	return Lux
        """

def init():
	pass
