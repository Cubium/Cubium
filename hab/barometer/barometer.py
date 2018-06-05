# code adapted for the MS5607 from
# https://github.com/ControlEverythingCommunity/MS5803-02Ba/blob/master/Python/MS5803_02BA.py

# NOTE:
# potential altitude calculation would be difficult as most formulas
# require calculation of a sea-level reference based on current
# atmospheric conditions, such that the sensor's code would have to be
# recalibrated whenever the weather changes; a rough estimate could
# potentially be achieved, but such a venture will not be persued at
# this time

# concerning the calculations of altitude found in the code below, it is
# rather a calculation of the change in altitude from some base

# NOTE:
# If the barometer is not appropriately connected, the getters will return only
# the following values:
# getPressure()         returns 0.00
# getTemperatureC()     returns 20.0
# getTemperatureF()     returns 68.0
# getAltitude()         returns 0.00

import math
import smbus
import time

bus = smbus.SMBus(1)

DEVICE_ADDRESS = 0x76

class barometer:

    def __init__(self):
        self.resetCommand()

        self.dT     = 0
        self.TEMP   = 0
        self.OFF    = 0
        self.SENS   = 0 
        self.C      = self.readCalibrations()

        self.pressure = 0  # in millibars
        self.tempC    = 0  # in degrees C
        self.tempF    = 0  # in degrees F

        time.sleep(0.003)

        # for base altitude calculation
        self.readBaro()
        self.base_altitude = self.calculateAltitude(self.pressure)
        self.change_altitude = 0

    def readBaro(self):
        # read digital pressure value
        self.pressureConversionCommand()
        self.D1 = self.readDigitalValue()

        # read digital temperature value
        self.temperatureConversionCommand()
        self.D2 = self.readDigitalValue()

        # pressure and temperature calculations
        self.dT   = self.D2 - self.C[4] * 256
        self.TEMP = 2000 + self.dT * self.C[5] / 8388608
        self.OFF  = self.C[1] * 131072 + (self.C[3] * self.dT) / 64
        self.SENS = self.C[0] * 65536 + (self.C[2] * self.dT) / 128

        TEMP2 = 0
        OFF2  = 0
        SENS2 = 0

        if self.TEMP < 2000:
            TEMP2 = (self.dT * self.dT) / 2147483648
            OFF2  = 61 * ((self.TEMP - 2000) ** 2) / 16
            SENS2 = 2 * ((self.TEMP - 2000) ** 2)
            if self.TEMP < -1500:
                OFF2  = OFF2 + 20 * ((self.TEMP + 1500) ** 2)
                SENS2 = SENS2 + 12 * ((self.TEMP + 1500) ** 2)

        self.TEMP -= TEMP2
        self.OFF  -= OFF2
        self.SENS -= SENS2

        self.pressure = ((((self.D1 * self.SENS) / 2097152) - self.OFF) / 32768.0) / 100.0
        self.tempC    = self.TEMP / 100.0
        self.tempF    = self.tempC * 1.8 + 32

    # UTILITY FUNCTIONS
    def readCalibrations(self): 
        coefficients = [0] * 6
        
        # pressure sensitivity
        coefficients[0] = self.readCalibration(0xA2)

        # pressure offset
        coefficients[1] = self.readCalibration(0xA4)

        # temperature coefficient of pressure sensitivity
        coefficients[2] = self.readCalibration(0xA6)

        # temperature coefficient of pressure offset
        coefficients[3] = self.readCalibration(0xA8)

        # reference temperature
        coefficients[4] = self.readCalibration(0xAA)

        # temperature coefficient of temperature
        coefficients[5] = self.readCalibration(0xAC)

        return coefficients

    def readCalibration(self, coefficient):
        # error handling
        try:
            data = bus.read_i2c_block_data(DEVICE_ADDRESS, coefficient, 2)
            return data[0] * 256 + data[1]
        except:
            return 0

    def readDigitalValue(self):
        # error handling
        try:
            value = bus.read_i2c_block_data(DEVICE_ADDRESS, 0x00, 3)
            return value[0] * 65536 + value[1] * 256 + value[2]
        except:
            return 0

    def resetCommand(self):
        # error handling
        try:
            bus.write_byte(DEVICE_ADDRESS, 0x1E)
        except:
            pass
        time.sleep(0.003)

    def pressureConversionCommand(self):
        # error handling
        try:
            bus.write_byte(DEVICE_ADDRESS, 0x40)
        except:
            pass
        time.sleep(0.003)

    def temperatureConversionCommand(self):
        # error handling
        try:
            bus.write_byte(DEVICE_ADDRESS, 0x50)
        except:
            pass
        time.sleep(0.003)

    # TODO: test this calculation
    def calculateAltitude(self, pressure):
        altitude = 1 - math.pow((pressure / 1013.25), 0.190264)
        altitude *= 44330.76923
        return altitude

    # GETTER FUNCTIONS
    def getPressure(self):
        return self.pressure

    def getTemperatureC(self):
        return self.tempC

    def getTemperatureF(self):
        return self.tempF

    def getAltitude(self, pressure):
        self.change_altitude = self.calculateAltitude(pressure)
        self.change_altitude -= self.base_altitude
        return self.change_altitude

