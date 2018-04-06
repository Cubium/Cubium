import time
import Adafruit_ADS1x15
import math

light =Adafruit_ADS1x15.ADS1115()

GAIN = 1

def handleSpaData():
    pass

def sendData():
	values = light.read_adc(2, gain=GAIN)

	values = values * .0048
	RLDR = (1000.0 * (5-values))/values
	#Lux = (776897.0 * (math.pow(RLDR, -1.206)))
	
	time.sleep(0.5)
	
	return values

	

def init():
    # Handled elsewhere
	pass
