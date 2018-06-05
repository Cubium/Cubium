from barometer import barometer

baro_sensor = barometer()

def init():
    pass

def handleSpaData():
    pass

def sendData():
    baro_sensor.readBaro()
    return baro_sensor.getPressure()
