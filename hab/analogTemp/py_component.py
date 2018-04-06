import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008
import time

CLK = 18
MISO = 23
MOSI = 24
CS = 25

analTemp = Adafruit_MCP3008.MCP3008(clk=CLK, cs=CS, miso=MISO, mosi=MOSI)

THERMISTORNOMINAL = 10000

TEMPERATURENOMINAL = 25

NUMSAMPLES = 5

BCOEFFICIENT = 3950

SERIESRESISTOR = 10000

samples = [] 



def handleSpaData():
    pass

def sendData():
    time.sleep(1)
    for i in range(0, NUMSAMPLES):
        samples.append(analTemp.read_adc(0))
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



