import Adafruit_BBIO.ADC as ADC
ADC.setup()

def handleSpaData():
    pass

def init():
    pass

def sendData():
    #Needs to read twice due to bug
    _           = ADC.read("AIN1")
    analoginput = ADC.read("AIN1")

    #convert from proportional (0-1.8V) to actual voltage
    voltage = analoginput * 1.8
    
    #convert from voltage to UV Index
    uvIndex = voltage * 10

    return float(uvIndex)