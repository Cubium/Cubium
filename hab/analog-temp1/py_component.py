import sys
sys.path.insert(0, 'Cubium/drivers/PythonDrivers')

import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008
import time
import math

SPI_PORT = 0
SPI_DEVICE = 0

analogTempFloor = Adafruit_MCP3008.MCP3008(spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE))

A = 0.002197222470870
B = 0.000161097632222
C = 0.000000125008328

PIN = 5

#implementation found here: https://bitbucket.org/pschow/rpiadctherm/src/dbfe8101eeb4/basiclogmcp.py?at=master&fileviewer=file-view-default

def handleSpaData():
    pass

def sendData():
    time.sleep(1)
    
    value = analogTempFloor.read_adc(PIN)
    volts = (value*3.3)/1024
    ohms = ((1/volts)*3300)-1000

    lnohm = math.log1p(ohms)

    t1 = (B*lnohm)
    c2 = C*lnohm
    t2 = math.pow(c2,3)

    temp = 1/(A+t1+t2)
    tempc = temp - 273.15 - 4

    return tempc

def init():
    pass
