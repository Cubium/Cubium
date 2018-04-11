import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008
import time

SPI_PORT = 0
SPI_DEVICE = 0

analogTempAvg = Adafruit_MCP3008.MCP3008(spi=SPI.spiDev(SPI_PORT, SPI_DEVICE))

THERMISTORNOMINAL = 10000
TEMPERATURENOMINAL = 25
NUMSAMPLES = 5
BCOEFFICIENT = 3950
SERIESRESISTOR = 10000

samples = [] 

WALL_PIN = 4
FLOOR_PIN = 5


def handleSpaData():
    pass

def sendData():
    time.sleep(1)
    for i in range(0, NUMSAMPLES):
        samples.append((analogTempAvg.read_adc(WALL_PIN)+analogTempAvg.read_adc(FLOOR_PIN))/2)
        time.sleep(.01)
    
    average = 0

    for i in range(0, NUMSAMPLES):
        average += samples[i]
        

    average /= NUMSAMPLES

    average = 1023/ average - 1

    average = SERIESRESISTOR / average

    steinhart = average / THERMISTORNOMINAL
    steinhart = math.log(steinhart)
    steinhart /= BCOEFFICIENT 
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15)  
    steinhart = 1.0 / steinhart
    steinhart -= 273.15
    
    return steinhart

def init():
    pass
