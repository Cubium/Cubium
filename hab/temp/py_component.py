import Adafruit_MCP9809 as MCP9809

def handleSpaData():
    pass

def sendData():
    return sensor.readTempC()

def init():
    sensor = MCP9809.MCP9809()
    sensor.begin()
