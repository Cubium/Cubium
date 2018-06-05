"""
Chronodot / DS3231 Real Time Clock Driver
"""

#import smbus

#bus = smbus.SMBus(1)
#address = 0x68

i = 0

def handleSpaData():
    """
    This doesn't do anything yet, we're going to leave it.
    """
    pass

def sendData():
    global i
    i += 1
    return "Time" + str(i) + "!"

def init():
    """
    this runs things once, before data is sent.

    We'll use this to initialize the SMBus/I2C library so it knows how to talk to the chip.
    """
