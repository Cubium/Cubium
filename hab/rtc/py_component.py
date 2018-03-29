"""
Chronodot / DS3231 Real Time Clock Driver
"""

import smbus
import time

bus = smbus.SMBus(1)
address = 0x68

def handleSpaData():
    """
    This doesn't do anything yet, we're going to leave it.
    """
    pass

def sendData():
    """
    http://bradsrpi.blogspot.com/2013/04/reading-time-date-from-chronodot-using.html
    chronodot uses the same chip

    """
    time.sleep(1)
    data = bus.read_i2c_block_data(address, 0)
    ss = (data[0]/16*10) + (data[0]%16)
    mm = (data[1]/16*10) + (data[1]%16)
    hr = (data[2]/16*10) + (data[2]%16)
    day = (data[3]/16*10) + (data[3]%16)
    date = (data[4]/16*10) + (data[4]%16)
    month = (data[5]/16*10) + (data[5]%16)
    year = (data[6]/16*10) + (data[6]%16)
    return "%02d:%02d:%02d %02d/%02d/%02d" % (hr, mm, ss, month, day, year)

def init():
    """
    this runs things once, before data is sent.

    We'll use this to initialize the SMBus/I2C library so it knows how to talk to the chip.
    """
