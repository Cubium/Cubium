#import Adafruit_GPIO.SPI as SPI
#import Adafruit_MCP3008


#SPI_PORT = 0
#SPI_DEVICE = 0
#uv_internal = Adafruit_MCP3008.MCP3008(spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE))
#ADC_PIN = 0


i = 0.0

def handleSpaData():
    pass

def sendData():
    global i
    i += 1.0
    print "Returning",420.0 + i
    return (420.0 + i)

def init():
    pass

print sendData()
