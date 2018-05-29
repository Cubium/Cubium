import Adafruit_MCP9808.MCP9808 as MCP9808

sensor = MCP9808.MCP9808(address=0x18, busnum=1)


def handleSpaData():
    pass

def sendData():
    return sensor.readTempC()

def init():
    sensor.begin()
    pass

