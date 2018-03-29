from ../../lib/python/Adafruit_MCP9808 import MCP9808
import time

sensor = MCP9808.MCP9808()

def handleSpaData():
    pass

def sendData():
    time.sleep(1)
    print sensor.readTempC()
    return sensor.readTempC()

def init():
    sensor.begin()
