import time
import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008

SPI_PORT = 0
SPI_DEVICE = 0
lightSensor = Adafruit_MCP3008.MCP3008(spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE))
LIGHTSENSOR1PIN = 2
LIGHTSENSOR2PIN = 3

def handleSpaData():
	pass

def sendData():
	lightSensor1 = lightSensor.read_adc(LIGHTSENSOR1PIN)
	lightSensor2 = lightSensor.read_adc(LIGHTSENSOR2PIN)

	values = (lightSensor1 + lightSensor2)/2
	values = values * .0048
	RLDR = (1000.0 * (5.0-values))/values
	Lux = (776897.0 * (math.pow(RLDR, -1.206)))

	time.sleep(1)

	return values

def init():
	pass
