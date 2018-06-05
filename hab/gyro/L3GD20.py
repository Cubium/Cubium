#!/usr/bin/python

from smbus import SMBus
import bitOps
import numpy
import time

class L3GD20(object):
    
    def __init__(self, busId, slaveAddr, ifLog, ifWriteBlock):
        self.__i2c = SMBus(busId)
        self.__slave = slaveAddr
        self.__ifWriteBlock = ifWriteBlock
        self.__ifLog = ifLog
        self.__x0 = 0
        
    def __del__(self):
        del(self.__i2c)

    def __log(self, register, mask, current, new):
        register   = '0b' + bin(register)[2:].zfill(8)
        mask       = '0b' + bin(mask)[2:].zfill(8)
        current    = '0b' + bin(current)[2:].zfill(8)
        new        = '0b' + bin(new)[2:].zfill(8)
        print('Change in register:' + register + ' mask:' + mask + ' from:' + current + ' to:' + new)
        
    def __writeToRegister(self, register, mask, value):
        current = self.__i2c.read_byte_data(self.__slave, register)  # Get current value
        new = bitOps.SetValueUnderMask(value, current, mask)
        if self.__ifLog:
            self.__log(register, mask, current, new)
        if  not self.__ifWriteBlock:
            self.__i2c.write_byte_data(self.__slave, register, new)
        
    def __readFromRegister(self, register, mask):
        current = self.__i2c.read_byte_data(self.__slave, register)   # Get current value
        return bitOps.GetValueUnderMask(current, mask)

    
    def __readFromRegisterWithDictionaryMatch(self, register, mask, dictionary):
        current = self.__readFromRegister(register, mask)
        for key in dictionary.keys():
            if dictionary[key] == current:
                return key
            
    def __writeToRegisterWithDictionaryCheck(self, register, mask, value, dictionary, dictionaryName):    
        if value not in dictionary.keys():
            raise Exception('Value:' + str(value) + ' is not in range of: ' + str(dictionaryName))
        self.__writeToRegister(register, mask, dictionary[value])
    
    
    
    __REG_R_WHO_AM_I            = 0x0f      # Device identification register
    __REG_RW_CTRL_REG1          = 0x20      # Control register 1
    __REG_RW_CTRL_REG2          = 0x21      # Control register 2
    __REG_RW_CTRL_REG3          = 0x22      # Control register 3
    __REG_RW_CTRL_REG4          = 0x23      # Control register 4
    __REG_RW_CTRL_REG5          = 0x24      # Control register 5
    __REG_RW_REFERENCE          = 0x25      # Reference value for interrupt generation
    __REG_R_OUT_TEMP            = 0x26      # Output temperature
    __REG_R_STATUS_REG          = 0x27      # Status register
    __REG_R_OUT_X_L             = 0x28      # X-axis angular data rate LSB
    __REG_R_OUT_X_H             = 0x29      # X-axis angular data rate MSB
    __REG_R_OUT_Y_L             = 0x2a      # Y-axis angular data rate LSB
    __REG_R_OUT_Y_H             = 0x2b      # Y-axis angular data rate MSB
    __REG_R_OUT_Z_L             = 0x2c      # Z-axis angular data rate LSB
    __REG_R_OUT_Z_H             = 0x2d      # Z-axis angular data rate MSB
    __REG_RW_FIFO_CTRL_REG      = 0x2e      # Fifo control register
    __REG_R_FIFO_SRC_REG        = 0x2f      # Fifo src register
    __REG_RW_INT1_CFG_REG       = 0x30      # Interrupt 1 configuration register
    __REG_R_INT1_SRC_REG        = 0x31      # Interrupt source register
    __REG_RW_INT1_THS_XH        = 0x32      # Interrupt 1 threshold level X MSB register
    __REG_RW_INT1_THS_XL        = 0x33      # Interrupt 1 threshold level X LSB register
    __REG_RW_INT1_THS_YH        = 0x34      # Interrupt 1 threshold level Y MSB register
    __REG_RW_INT1_THS_YL        = 0x35      # Interrupt 1 threshold level Y LSB register
    __REG_RW_INT1_THS_ZH        = 0x36      # Interrupt 1 threshold level Z MSB register
    __REG_RW_INT1_THS_ZL        = 0x37      # Interrupt 1 threshold level Z LSB register
    __REG_RW_INT1_DURATION      = 0x38      # Interrupt 1 duration register
    
    __MASK_CTRL_REG1_Xen        = 0x01      # X enable
    __MASK_CTRL_REG1_Yen        = 0x02      # Y enable
    __MASK_CTRL_REG1_Zen        = 0x04      # Z enable
    __MASK_CTRL_REG1_PD         = 0x08      # Power-down
    __MASK_CTRL_REG1_BW         = 0x30      # Bandwidth
    __MASK_CTRL_REG1_DR         = 0xc0      # Output data rate
    __MASK_CTRL_REG2_HPCF       = 0x0f      # High pass filter cutoff frequency
    __MASK_CTRL_REG2_HPM        = 0x30      # High pass filter mode selection
    __MASK_CTRL_REG3_I2_EMPTY   = 0x01      # FIFO empty interrupt on DRDY/INT2
    __MASK_CTRL_REG3_I2_ORUN    = 0x02      # FIFO overrun interrupt on DRDY/INT2
    __MASK_CTRL_REG3_I2_WTM     = 0x04      # FIFO watermark interrupt on DRDY/INT2
    __MASK_CTRL_REG3_I2_DRDY    = 0x08      # Date-ready on DRDY/INT2
    __MASK_CTRL_REG3_PP_OD      = 0x10      # Push-pull / Open-drain
    __MASK_CTRL_REG3_H_LACTIVE  = 0x20      # Interrupt active configuration on INT1
    __MASK_CTRL_REG3_I1_BOOT    = 0x40      # Boot status available on INT1
    __MASK_CTRL_REG3_I1_Int1    = 0x80      # Interrupt enabled on INT1
    __MASK_CTRL_REG4_SIM        = 0x01      # SPI Serial interface selection
    __MASK_CTRL_REG4_FS         = 0x30      # Full scale selection
    __MASK_CTRL_REG4_BLE        = 0x40      # Big/little endian selection
    __MASK_CTRL_REG4_BDU        = 0x80      # Block data update
    __MASK_CTRL_REG5_OUT_SEL    = 0x03      # Out selection configuration
    __MASK_CTRL_REG5_INT_SEL    = 0xc0      # INT1 selection configuration
    __MASK_CTRL_REG5_HPEN       = 0x10      # High-pass filter enable
    __MASK_CTRL_REG5_FIFO_EN    = 0x40      # Fifo enable
    __MASK_CTRL_REG5_BOOT       = 0x80      # Reboot memory content
    __MASK_STATUS_REG_ZYXOR     = 0x80      # Z, Y, X axis overrun
    __MASK_STATUS_REG_ZOR       = 0x40      # Z axis overrun
    __MASK_STATUS_REG_YOR       = 0x20      # Y axis overrun
    __MASK_STATUS_REG_XOR       = 0x10      # X axis overrun
    __MASK_STATUS_REG_ZYXDA     = 0x08      # Z, Y, X data available
    __MASK_STATUS_REG_ZDA       = 0x04      # Z data available
    __MASK_STATUS_REG_YDA       = 0x02      # Y data available
    __MASK_STATUS_REG_XDA       = 0x01      # X data available
    __MASK_FIFO_CTRL_REG_FM     = 0xe0      # Fifo mode selection
    __MASK_FIFO_CTRL_REG_WTM    = 0x1f      # Fifo treshold - watermark level
    __MASK_FIFO_SRC_REG_FSS     = 0x1f      # Fifo stored data level
    __MASK_FIFO_SRC_REG_EMPTY   = 0x20      # Fifo empty bit
    __MASK_FIFO_SRC_REG_OVRN    = 0x40      # Overrun status
    __MASK_FIFO_SRC_REG_WTM     = 0x80      # Watermark status
    __MASK_INT1_CFG_ANDOR       = 0x80      # And/Or configuration of interrupt events 
    __MASK_INT1_CFG_LIR         = 0x40      # Latch interrupt request
    __MASK_INT1_CFG_ZHIE        = 0x20      # Enable interrupt generation on Z high
    __MASK_INT1_CFG_ZLIE        = 0x10      # Enable interrupt generation on Z low
    __MASK_INT1_CFG_YHIE        = 0x08      # Enable interrupt generation on Y high
    __MASK_INT1_CFG_YLIE        = 0x04      # Enable interrupt generation on Y low
    __MASK_INT1_CFG_XHIE        = 0x02      # Enable interrupt generation on X high
    __MASK_INT1_CFG_XLIE        = 0x01      # Enable interrupt generation on X low
    __MASK_INT1_SRC_IA          = 0x40      # Int1 active
    __MASK_INT1_SRC_ZH          = 0x20      # Int1 source Z high
    __MASK_INT1_SRC_ZL          = 0x10      # Int1 source Z low
    __MASK_INT1_SRC_YH          = 0x08      # Int1 source Y high
    __MASK_INT1_SRC_YL          = 0x04      # Int1 source Y low
    __MASK_INT1_SRC_XH          = 0x02      # Int1 source X high
    __MASK_INT1_SRC_XL          = 0x01      # Int1 source X low  
    __MASK_INT1_THS_H           = 0x7f      # MSB
    __MASK_INT1_THS_L           = 0xff      # LSB
    __MASK_INT1_DURATION_WAIT   = 0x80      # Wait number of samples or not
    __MASK_INT1_DURATION_D      = 0x7f      # Duration of int1 to be recognized
     
    PowerModeEnum = [ 'Power-down', 'Sleep', 'Normal']
    __PowerModeDict = { PowerModeEnum[0] : 0, PowerModeEnum[1] : 1, PowerModeEnum[2] : 2 }
    
    EnabledEnum = [ False, True ]
    __EnabledDict = { EnabledEnum[0] : 0, EnabledEnum[1] : 1}
    
    LevelEnum = [ 'High', 'Low' ]
    __LevelDict = { LevelEnum[0] : 0, LevelEnum[1] : 1 }
    
    OutputEnum = [ 'Push-pull', 'Open drain' ]
    __OutputDict = { OutputEnum[0] : 0, OutputEnum[1] : 1 }
    
    SimModeEnum = [ '4-wire', '3-wire' ]
    __SimModeDict = { SimModeEnum[0] : 0, SimModeEnum[1] : 1 }
    
    FullScaleEnum = [ '250dps', '500dps', '2000dps' ]
    __FullScaleDict = { FullScaleEnum[0] : 0x00, FullScaleEnum[1] : 0x01, FullScaleEnum[2] : 0x02}
    
    BigLittleEndianEnum = [ 'Big endian', 'Little endian' ]
    __BigLittleEndianDict = { BigLittleEndianEnum[0] : 0x00, BigLittleEndianEnum[1] : 0x01 }
    
    BlockDataUpdateEnum = [ 'Continous update', 'Output registers not updated until reading' ]
    __BlockDataUpdateDict = { BlockDataUpdateEnum[0] : 0x00, BlockDataUpdateEnum[1] : 0x01 }
    
    OutSelEnum = [ 'LPF1', 'HPF', 'LPF2' ]
    __OutSelDict = { OutSelEnum[0] : 0x00, OutSelEnum[1] : 0x01, OutSelEnum[2] : 0x02 }
    
    IntSelEnum = [ 'LPF1', 'HPF', 'LPF2' ]
    __IntSelDict = { IntSelEnum[0] : 0x00, IntSelEnum[1] : 0x01, IntSelEnum[2] : 0x02 }
    
    BootModeEnum = [ 'Normal', 'Reboot memory content' ]
    __BootModeDict = { BootModeEnum[0] : 0x00, BootModeEnum[1] : 0x01 }
    
    FifoModeEnum = [ 'Bypass', 'FIFO', 'Stream', 'Stream-to-Fifo', 'Bypass-to-Stream' ]
    __FifoModeDict = {
        FifoModeEnum[0] : 0x00,
        FifoModeEnum[1] : 0x01,
        FifoModeEnum[2] : 0x02,
        FifoModeEnum[3] : 0x03,
        FifoModeEnum[4] : 0x04
    }
    
    AndOrEnum = [ 'And', 'Or' ]
    __AndOrDict = { AndOrEnum[0] : 0x00, AndOrEnum[1] : 0x01 }

    DataRateValues = [95, 190, 380, 760]
    BandWidthValues = [12.5, 20, 25, 30, 35, 50, 70, 100]
    __DRBW = { 
        DataRateValues[0]  : { BandWidthValues[0]:0x00, BandWidthValues[2]:0x01},
        DataRateValues[1] : { BandWidthValues[0]:0x04, BandWidthValues[2]:0x05, BandWidthValues[5]:0x06, BandWidthValues[6]:0x07},
        DataRateValues[2] : { BandWidthValues[1]:0x08, BandWidthValues[2]:0x09, BandWidthValues[5]:0x0a, BandWidthValues[7]:0x0b},
        DataRateValues[3] : { BandWidthValues[3]:0x0c, BandWidthValues[4]:0x0d, BandWidthValues[5]:0x0e, BandWidthValues[7]:0x0f}
    }
    
    HighPassFilterCutOffFrequencyValues = [51.4, 27, 13.5, 7.2, 3.5, 1.8, 0.9, 0.45, 0.18, 0.09, 0.045, 0.018, 0.009]
    __HPCF = {
        HighPassFilterCutOffFrequencyValues[0]  : { DataRateValues[3]:0x00 },
        HighPassFilterCutOffFrequencyValues[1]  : { DataRateValues[2]:0x00, DataRateValues[3]:0x01 },
        HighPassFilterCutOffFrequencyValues[2]  : { DataRateValues[1]:0x00, DataRateValues[2]:0x01, DataRateValues[3]:0x02 },
        HighPassFilterCutOffFrequencyValues[3]  : { DataRateValues[0]:0x00, DataRateValues[1]:0x01, DataRateValues[2]:0x02, DataRateValues[3]:0x03 },
        HighPassFilterCutOffFrequencyValues[4]  : { DataRateValues[0]:0x01, DataRateValues[1]:0x02, DataRateValues[2]:0x03, DataRateValues[3]:0x04 },
        HighPassFilterCutOffFrequencyValues[5]  : { DataRateValues[0]:0x02, DataRateValues[1]:0x03, DataRateValues[2]:0x04, DataRateValues[3]:0x05 },
        HighPassFilterCutOffFrequencyValues[6]  : { DataRateValues[0]:0x03, DataRateValues[1]:0x04, DataRateValues[2]:0x05, DataRateValues[3]:0x06 },
        HighPassFilterCutOffFrequencyValues[7]  : { DataRateValues[0]:0x04, DataRateValues[1]:0x05, DataRateValues[2]:0x06, DataRateValues[3]:0x07 },
        HighPassFilterCutOffFrequencyValues[8]  : { DataRateValues[0]:0x05, DataRateValues[1]:0x06, DataRateValues[2]:0x07, DataRateValues[3]:0x08 },
        HighPassFilterCutOffFrequencyValues[9]  : { DataRateValues[0]:0x06, DataRateValues[1]:0x07, DataRateValues[2]:0x08, DataRateValues[3]:0x09 },
        HighPassFilterCutOffFrequencyValues[10] : { DataRateValues[0]:0x07, DataRateValues[1]:0x08, DataRateValues[2]:0x09 },
        HighPassFilterCutOffFrequencyValues[11] : { DataRateValues[0]:0x08, DataRateValues[1]:0x09 },
        HighPassFilterCutOffFrequencyValues[12] : { DataRateValues[0]:0x09 }
    }
    
    HighPassFilterModes = ['Normal with reset.','Reference signal for filtering.','Normal.','Autoreset on interrupt.']
    __HpmDict = {
        HighPassFilterModes[0]:0x0,
        HighPassFilterModes[1]:0x1,
        HighPassFilterModes[2]:0x2,
        HighPassFilterModes[3]:0x3
    }
    
    # For calibration purposes
    meanX = 0
    maxX = 0
    minX = 0
    meanY = 0
    maxY = 0
    minY = 0
    meanZ = 0
    maxZ = 0
    minZ = 0
    
    gain = 1
    
    
    def Init(self):
        """Call this method after configuratin and before doing measurements"""
        print("Initiating...")
        if (self.Get_FullScale_Value() == self.FullScaleEnum[0]):
            self.gain = 0.00875
        elif (self.Get_FullScale_Value() == self.FullScaleEnum[1]):
            self.gain = 0.0175
        elif (self.Get_FullScale_Value() == self.FullScaleEnum[2]):
            self.gain = 0.07
        print("Gain set to:{0}".format(self.gain))


    def CalibrateX(self):
        """Returns (min, mean, max)"""
        print("Calibrating axis X, please do not move sensor...")
        buff = []
        for t in range(20):
            while self.Get_AxisDataAvailable_Value()[0] == 0:
                time.sleep(0.0001)
            buff.append(self.Get_RawOutX_Value())
        self.meanX = numpy.mean(buff) 
        self.maxX = max(buff)
        self.minX = min(buff)
        print("Done: (min={0};mean={1};max={2})".format(self.minX, self.meanX, self.maxX))
        
    def CalibrateY(self):
        """Returns (min, mean, max)"""
        print("Calibrating axis Y, please do not move sensor...")
        buff = []
        for t in range(20):
            while self.Get_AxisDataAvailable_Value()[1] == 0:
                time.sleep(0.0001)
            buff.append(self.Get_RawOutY_Value())
        self.meanY = numpy.mean(buff) 
        self.maxY = max(buff)
        self.minY = min(buff)
        print("Done: (min={0};mean={1};max={2})".format(self.minY, self.meanY, self.maxY))
        
    def CalibrateZ(self):
        """Returns (min, mean, max)"""
        print("Calibrating axis Z, please do not move sensor...")
        buff = []
        for t in range(20):
            while self.Get_AxisDataAvailable_Value()[2] == 0:
                time.sleep(0.0001)
            buff.append(self.Get_RawOutZ_Value())
        self.meanZ = numpy.mean(buff) 
        self.maxZ = max(buff)
        self.minZ = min(buff)
        print("Done: (min={0};mean={1};max={2})".format(self.minZ, self.meanZ, self.maxZ))

    def Calibrate(self):
		self.CalibrateX()
		self.CalibrateY()
		self.CalibrateZ()
            
    def ReturnConfiguration(self):
        return  [
            [ self.Get_DeviceId_Value.__doc__, self.Get_DeviceId_Value()],
            
            [ self.Get_DataRateAndBandwidth.__doc__, self.Get_DataRateAndBandwidth()],
            [ self.Get_AxisX_Enabled.__doc__, self.Get_AxisX_Enabled()],
            [ self.Get_AxisY_Enabled.__doc__, self.Get_AxisY_Enabled()],
            [ self.Get_AxisZ_Enabled.__doc__, self.Get_AxisZ_Enabled()],
            
            [ self.Get_PowerMode.__doc__, self.Get_PowerMode()],
            [ self.Get_HighPassCutOffFreq.__doc__, self.Get_HighPassCutOffFreq()],
            
            [ self.Get_INT1_Enabled.__doc__, self.Get_INT1_Enabled()],
            [ self.Get_BootStatusOnINT1_Enabled.__doc__, self.Get_BootStatusOnINT1_Enabled()],
            [ self.Get_ActiveConfINT1_Level.__doc__, self.Get_ActiveConfINT1_Level()],
            [ self.Get_PushPullOrOpenDrain_Value.__doc__, self.Get_PushPullOrOpenDrain_Value()],
            [ self.Get_DataReadyOnINT2_Enabled.__doc__, self.Get_DataReadyOnINT2_Enabled()],
            [ self.Get_FifoWatermarkOnINT2_Enabled.__doc__, self.Get_FifoWatermarkOnINT2_Enabled()],
            [ self.Get_FifoOverrunOnINT2_Enabled.__doc__, self.Get_FifoOverrunOnINT2_Enabled()],
            [ self.Get_FifoEmptyOnINT2_Enabled.__doc__, self.Get_FifoEmptyOnINT2_Enabled()],
            
            [ self.Get_SpiMode_Value.__doc__, self.Get_SpiMode_Value()],
            [ self.Get_FullScale_Value.__doc__, self.Get_FullScale_Value()],
            [ self.Get_BigLittleEndian_Value.__doc__, self.Get_BigLittleEndian_Value()],
            [ self.Get_BlockDataUpdate_Value.__doc__, self.Get_BlockDataUpdate_Value()],
            
            [ self.Get_BootMode_Value.__doc__, self.Get_BootMode_Value()],
            [ self.Get_Fifo_Enabled.__doc__, self.Get_Fifo_Enabled()],
            [ self.Get_HighPassFilter_Enabled.__doc__, self.Get_HighPassFilter_Enabled()],
            [ self.Get_INT1Selection_Value.__doc__, self.Get_INT1Selection_Value()],
            [ self.Get_OutSelection_Value.__doc__, self.Get_OutSelection_Value()],
            
            [ self.Get_Reference_Value.__doc__, self.Get_Reference_Value()],
            
            [ self.Get_AxisOverrun_Value.__doc__, self.Get_AxisOverrun_Value()],
            
            [ self.Get_AxisDataAvailable_Value.__doc__, self.Get_AxisDataAvailable_Value()],
            
            [ self.Get_FifoThreshold_Value.__doc__, self.Get_FifoThreshold_Value()],
            [ self.Get_FifoMode_Value.__doc__, self.Get_FifoMode_Value()],
            
            [ self.Get_FifoStoredDataLevel_Value.__doc__, self.Get_FifoStoredDataLevel_Value()],
            [ self.Get_IsFifoEmpty_Value.__doc__, self.Get_IsFifoEmpty_Value()],
            [ self.Get_IsFifoFull_Value.__doc__, self.Get_IsFifoFull_Value()],
            [ self.Get_IsFifoGreaterOrEqualThanWatermark_Value.__doc__, self.Get_IsFifoGreaterOrEqualThanWatermark_Value()],

            [ self.Get_Int1Combination_Value.__doc__, self.Get_Int1Combination_Value() ],
            [ self.Get_Int1LatchRequest_Enabled.__doc__, self.Get_Int1LatchRequest_Enabled() ],
            [ self.Get_Int1GenerationOnZHigh_Enabled.__doc__, self.Get_Int1GenerationOnZHigh_Enabled() ],
            [ self.Get_Int1GenerationOnZLow_Enabled.__doc__, self.Get_Int1GenerationOnZLow_Enabled() ],
            [ self.Get_Int1GenerationOnYHigh_Enabled.__doc__, self.Get_Int1GenerationOnYHigh_Enabled() ],
            [ self.Get_Int1GenerationOnYLow_Enabled.__doc__, self.Get_Int1GenerationOnYLow_Enabled() ],
            [ self.Get_Int1GenerationOnXHigh_Enabled.__doc__, self.Get_Int1GenerationOnXHigh_Enabled() ],
            [ self.Get_Int1GenerationOnXLow_Enabled.__doc__, self.Get_Int1GenerationOnXLow_Enabled() ],
            
            [ self.Get_Int1Active_Value.__doc__, self.Get_Int1Active_Value() ],
            [ self.Get_ZHighEventOccured_Value.__doc__, self.Get_ZHighEventOccured_Value() ],
            [ self.Get_ZLowEventOccured_Value.__doc__, self.Get_ZLowEventOccured_Value() ],
            [ self.Get_YHighEventOccured_Value.__doc__, self.Get_YHighEventOccured_Value() ],
            [ self.Get_YLowEventOccured_Value.__doc__, self.Get_YLowEventOccured_Value() ],
            [ self.Get_XHighEventOccured_Value.__doc__, self.Get_XHighEventOccured_Value() ],
            [ self.Get_XLowEventOccured_Value.__doc__, self.Get_XLowEventOccured_Value() ],
   
            [ self.Get_Int1Threshold_Values.__doc__, self.Get_Int1Threshold_Values() ],
            
            [ self.Get_Int1DurationWait_Enabled.__doc__, self.Get_Int1DurationWait_Enabled() ],
            [ self.Get_Int1Duration_Value.__doc__, self.Get_Int1Duration_Value() ]
    
                 ]
    
    
    def Get_DeviceId_Value(self):
        """Device Id."""
        return self.__readFromRegister(self.__REG_R_WHO_AM_I, 0xff)
    
    def Set_AxisX_Enabled(self, enabled):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG1, self.__MASK_CTRL_REG1_Xen, enabled, self.__EnabledDict, 'EnabledEnum')
    def Get_AxisX_Enabled(self):
        """Axis X enabled."""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG1, self.__MASK_CTRL_REG1_Xen, self.__EnabledDict)
            
    def Set_AxisY_Enabled(self, enabled):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG1, self.__MASK_CTRL_REG1_Yen, enabled, self.__EnabledDict, 'EnabledEnum')
    def Get_AxisY_Enabled(self):
        """Axis Y enabled."""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG1, self.__MASK_CTRL_REG1_Yen, self.__EnabledDict)
            
    def Set_AxisZ_Enabled(self, enabled):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG1, self.__MASK_CTRL_REG1_Zen, enabled, self.__EnabledDict, 'EnabledEnum')   
    def Get_AxisZ_Enabled(self):
        """Axis Z enabled."""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG1, self.__MASK_CTRL_REG1_Zen, self.__EnabledDict)
    
    def Set_PowerMode(self, mode):
        if mode not in self.__PowerModeDict.keys():
            raise Exception('Value:' + str(mode) + ' is not in range of: PowerModeEnum')
        if self.__PowerModeDict[mode] == 0:
            # Power-down
            self.__writeToRegister(self.__REG_RW_CTRL_REG1, self.__MASK_CTRL_REG1_PD, 0)
        elif self.__PowerModeDict[mode] == 1:
            # Sleep
            self.__writeToRegister(self.__REG_RW_CTRL_REG1, self.__MASK_CTRL_REG1_PD | self.__MASK_CTRL_REG1_Zen | self.__MASK_CTRL_REG1_Yen | self.__MASK_CTRL_REG1_Xen, 8)
        elif self.__PowerModeDict[mode] == 2:
            # Normal
            self.__writeToRegister(self.__REG_RW_CTRL_REG1, self.__MASK_CTRL_REG1_PD, 1)
    def Get_PowerMode(self):
        """Power mode."""
        powermode = self.__readFromRegister(self.__REG_RW_CTRL_REG1, self.__MASK_CTRL_REG1_PD | self.__MASK_CTRL_REG1_Xen | self.__MASK_CTRL_REG1_Yen | self.__MASK_CTRL_REG1_Zen)
        print(bin(powermode))
        dictval = 4
        if not bitOps.CheckBit(powermode, 3):
            dictval = 0
        elif powermode == 0b1000:
            dictval = 1
        elif bitOps.CheckBit(powermode, 3):
            dictval = 2
        for key in self.__PowerModeDict.keys():
            if self.__PowerModeDict[key] == dictval:
                return key

    def Print_DataRateAndBandwidth_AvailableValues(self):
        for dr in self.__DRBW.keys():
            print('Output data rate: ' + dr + '[Hz]')
            for bw in self.__DRBW[dr].keys():
                print('   Bandwidth: ' + bw + ' (DRBW=' +'0b' + bin(self.__DRBW[dr][bw])[2:].zfill(4) +')')
    def Set_DataRateAndBandwidth(self, datarate, bandwidth):
        if datarate not in self.__DRBW.keys():
            raise Exception('Data rate:' + str(datarate) + ' not in range of data rate values.')
        if bandwidth not in self.__DRBW[datarate].keys():
            raise Exception('Bandwidth: ' + str(bandwidth) + ' cannot be assigned to data rate: ' + str(datarate))
        bits = self.__DRBW[datarate][bandwidth]
        self.__writeToRegister(self.__REG_RW_CTRL_REG1, self.__MASK_CTRL_REG1_DR | self.__MASK_CTRL_REG1_BW, bits)
    def Get_DataRateAndBandwidth(self):
        """Data rate and bandwidth."""
        current = self.__readFromRegister(self.__REG_RW_CTRL_REG1, self.__MASK_CTRL_REG1_DR | self.__MASK_CTRL_REG1_BW)
        for dr in self.__DRBW.keys():
            for bw in self.__DRBW[dr].keys():
                if self.__DRBW[dr][bw] == current:
                    return (dr, bw)
                
    def Print_HighPassFilterCutOffFrequency_AvailableValues(self):
        for freq in self.__HPCF.keys():
            print('High pass cut off: ' + freq + '[Hz]')
            for odr in self.__HPCF[freq].keys():
                print('   Output data rate: ' + odr + ' (HPCF=' + '0b' + bin(self.__HPCF[freq][odr])[2:].zfill(4) + ')')            
    def Set_HighPassCutOffFreq(self, freq):
        if freq not in self.__HPCF.keys():
            raise Exception('Frequency:' + str(freq) + ' is not in range of high pass frequency cut off values.')
        datarate = self.Get_DataRateAndBandwidth()[0]
        if datarate not in self.__HPCF[freq].keys():
            raise Exception('Frequency: ' + str(freq) + ' cannot be assigned to data rate: ' + str(datarate))
        bits = self.__HPCF[freq][datarate]   
        self.__writeToRegister(self.__REG_RW_CTRL_REG2, self.__MASK_CTRL_REG2_HPCF, bits)
    def Get_HighPassCutOffFreq(self):
        """Cut off frequency."""
        current = self.__readFromRegister(self.__REG_RW_CTRL_REG2, self.__MASK_CTRL_REG2_HPCF)
        datarate = self.Get_DataRateAndBandwidth()[0]
        for freq in self.__HPCF.keys():
            for dr in self.__HPCF[freq]:
                if dr == datarate:
                    if self.__HPCF[freq][datarate] == current:
                        return freq
    
    def Set_HighPassFilterMode(self, mode):
        if mode not in self.__HpmDict.keys():
            raise Exception('EnabledEnum:' + str(mode) + ' is not in range of high pass frequency modes.')
        bits = self.__HpmDict[mode]
        self.__writeToRegister(self.__REG_RW_CTRL_REG2, self.__MASK_CTRL_REG2_HPM, bits)
    def Get_HighPassFilterMode(self):
        """High pass filter mode"""
        current = self.__readFromRegister(self.__REG_RW_CTRL_REG2, self.__MASK_CTRL_REG2_HPM)
        for mode in self.__HpmDict.keys():
            if self.__HpmDict[mode] == current:
                return mode

    def Set_INT1_Enabled(self, enabled):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_I1_Int1, enabled, self.__EnabledDict, 'EnabledEnum') 
    def Get_INT1_Enabled(self):
        """INT1 Enabled"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_I1_Int1, self.__EnabledDict)
    
    def Set_BootStatusOnINT1_Enabled(self, enabled):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_I1_BOOT, enabled, self.__EnabledDict, 'EnabledEnum') 
    def Get_BootStatusOnINT1_Enabled(self):
        """Boot status available on INT1"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_I1_BOOT, self.__EnabledDict)
    
    def Set_ActiveConfINT1_Level(self, level):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_H_LACTIVE, level, self.__LevelDict, 'LevelEnum') 
    def Get_ActiveConfINT1_Level(self):
        """Interrupt active configuration on INT1"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_H_LACTIVE, self.__LevelDict)
    
    def Set_PushPullOrOpenDrain_Value(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_PP_OD, value, self.__OutputDict, 'OutputEnum') 
    def Get_PushPullOrOpenDrain_Value(self):
        """Push-pull/open drain"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_PP_OD, self.__OutputDict)
    
    def Set_DataReadyOnINT2_Enabled(self, enabled):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_I2_DRDY, enabled, self.__EnabledDict, 'EnabledEnum') 
    def Get_DataReadyOnINT2_Enabled(self):
        """Date-ready on DRDY/INT2"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_I2_DRDY, self.__EnabledDict)
    
    def Set_FifoWatermarkOnINT2_Enabled(self, enabled):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_I2_WTM, enabled, self.__EnabledDict, 'EnabledEnum') 
    def Get_FifoWatermarkOnINT2_Enabled(self):
        """FIFO watermark interrupt on DRDY/INT2"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_I2_WTM, self.__EnabledDict)
    
    def Set_FifoOverrunOnINT2_Enabled(self, enabled):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_I2_ORUN, enabled, self.__EnabledDict, 'EnabledEnum') 
    def Get_FifoOverrunOnINT2_Enabled(self):
        """FIFO overrun interrupt in DRDY/INT2"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_I2_ORUN, self.__EnabledDict)
    
    def Set_FifoEmptyOnINT2_Enabled(self, enabled):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_I2_EMPTY, enabled, self.__EnabledDict, 'EnabledEnum') 
    def Get_FifoEmptyOnINT2_Enabled(self):
        """FIFO empty interrupt on DRDY/INT2"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG3, self.__MASK_CTRL_REG3_I2_EMPTY, self.__EnabledDict)
    
    def Set_SpiMode_Value(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG4, self.__MASK_CTRL_REG4_SIM, value, self.__SimModeDict, 'SimModeEnum') 
    def Get_SpiMode_Value(self):
        """SPI mode"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG4, self.__MASK_CTRL_REG4_SIM, self.__SimModeDict)
    
    def Set_FullScale_Value(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG4, self.__MASK_CTRL_REG4_FS, value, self.__FullScaleDict, 'FullScaleEnum') 
    def Get_FullScale_Value(self):
        """Full scale selection"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG4, self.__MASK_CTRL_REG4_FS, self.__FullScaleDict)
    
    def Set_BigLittleEndian_Value(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG4, self.__MASK_CTRL_REG4_BLE, value, self.__BigLittleEndianDict, 'BigLittleEndianEnum') 
    def Get_BigLittleEndian_Value(self):
        """Big/Little endian"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG4, self.__MASK_CTRL_REG4_BLE, self.__BigLittleEndianDict)
    
    def Set_BlockDataUpdate_Value(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG4, self.__MASK_CTRL_REG4_BDU, value, self.__BlockDataUpdateDict, 'BlockDataUpdateEnum') 
    def Get_BlockDataUpdate_Value(self):
        """Block data update"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG4, self.__MASK_CTRL_REG4_BDU, self.__BlockDataUpdateDict)
    
    def Set_BootMode_Value(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG5, self.__MASK_CTRL_REG5_BOOT, value, self.__BootModeDict, 'BootModeEnum') 
    def Get_BootMode_Value(self):
        """Boot mode"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG5, self.__MASK_CTRL_REG5_BOOT, self.__BootModeDict)

    def Set_Fifo_Enabled(self, enabled):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG5, self.__MASK_CTRL_REG5_FIFO_EN, enabled, self.__EnabledDict, 'EnabledEnum') 
    def Get_Fifo_Enabled(self):
        """Fifo enabled"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG5, self.__MASK_CTRL_REG5_FIFO_EN, self.__EnabledDict)
  
    def Set_HighPassFilter_Enabled(self, enabled):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG5, self.__MASK_CTRL_REG5_HPEN, enabled, self.__EnabledDict, 'EnabledEnum') 
    def Get_HighPassFilter_Enabled(self):
        """High pass filter enabled"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG5, self.__MASK_CTRL_REG5_HPEN, self.__EnabledDict)
        
    def Set_INT1Selection_Value(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG5, self.__MASK_CTRL_REG5_INT_SEL, value, self.__IntSelDict, 'IntSelEnum') 
    def Get_INT1Selection_Value(self):
        """INT1 selection configuration"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG5, self.__MASK_CTRL_REG5_INT_SEL, self.__IntSelDict)
    
    def Set_OutSelection_Value(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_CTRL_REG5, self.__MASK_CTRL_REG5_OUT_SEL, value, self.__OutSelDict, 'OutSelEnum') 
    def Get_OutSelection_Value(self):
        """Out selection configuration"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_CTRL_REG5, self.__MASK_CTRL_REG5_OUT_SEL, self.__OutSelDict)   
    
    def Set_Reference_Value(self, value):
        self.__writeToRegister(self.__REG_RW_REFERENCE, 0xff, value) 
    def Get_Reference_Value(self):
        """Reference value for interrupt generation"""
        return self.__readFromRegister(self.__REG_RW_REFERENCE, 0xff)
    
    def Get_OutTemp_Value(self):
        """Output temperature"""
        return self.__readFromRegister(self.__REG_R_OUT_TEMP, 0xff)
    
    def Get_AxisOverrun_Value(self):
        """(X, Y, Z) axis overrun"""
        zor = 0
        yor = 0
        xor = 0
        if self.__readFromRegister(self.__REG_R_STATUS_REG, self.__MASK_STATUS_REG_ZYXOR) == 0x01:
            zor = self.__readFromRegister(self.__REG_R_STATUS_REG, self.__MASK_STATUS_REG_ZOR)
            yor = self.__readFromRegister(self.__REG_R_STATUS_REG, self.__MASK_STATUS_REG_YOR)
            xor = self.__readFromRegister(self.__REG_R_STATUS_REG, self.__MASK_STATUS_REG_XOR)
        return (xor, yor, zor)
    
    def Get_AxisDataAvailable_Value(self):
        """(X, Y, Z) data available"""
        zda = 0
        yda = 0
        xda = 0
        if self.__readFromRegister(self.__REG_R_STATUS_REG, self.__MASK_STATUS_REG_ZYXDA) == 0x01:
            zda = self.__readFromRegister(self.__REG_R_STATUS_REG, self.__MASK_STATUS_REG_ZDA)
            yda = self.__readFromRegister(self.__REG_R_STATUS_REG, self.__MASK_STATUS_REG_YDA)
            xda = self.__readFromRegister(self.__REG_R_STATUS_REG, self.__MASK_STATUS_REG_XDA)
        return (xda, yda, zda)
    
    def Get_RawOutX_Value(self):
        """Raw X angular speed data"""
        l = self.__readFromRegister(self.__REG_R_OUT_X_L, 0xff)
        h_u2 = self.__readFromRegister(self.__REG_R_OUT_X_H, 0xff)
        h = bitOps.TwosComplementToByte(h_u2)
        if (h < 0):
            return (h*256 - l) * self.gain
        elif (h >= 0):
            return (h*256 + l) * self.gain
    
    def Get_RawOutY_Value(self):
        """Raw Y angular speed data"""
        l = self.__readFromRegister(self.__REG_R_OUT_Y_L, 0xff)
        h_u2 = self.__readFromRegister(self.__REG_R_OUT_Y_H, 0xff)
        h = bitOps.TwosComplementToByte(h_u2)
        if (h < 0):
            return (h*256 - l) * self.gain
        elif (h >= 0):
            return (h*256 + l) * self.gain
    
    def Get_RawOutZ_Value(self):
        """Raw Z angular speed data"""
        l = self.__readFromRegister(self.__REG_R_OUT_Z_L, 0xff)
        h_u2 = self.__readFromRegister(self.__REG_R_OUT_Z_H, 0xff)
        h = bitOps.TwosComplementToByte(h_u2)
        if (h < 0):
            return (h*256 - l) * self.gain
        elif (h >= 0):
            return (h*256 + l) * self.gain

    def Get_RawOut_Value(self):
        """Raw [X, Y, Z] values of angular speed"""
        return [self.Get_RawOutX_Value(), self.Get_RawOutY_Value(), self.Get_RawOutZ_Value()]
        
    def Get_CalOutX_Value(self):
        """Calibrated X angular speed data"""
        x = self.Get_RawOutX_Value()
        if(x >= self.minX and x <= self.maxX):
            return 0
        else:
            return x - self.meanX
            
    def Get_CalOutY_Value(self):
        """Calibrated Y angular speed data"""
        y = self.Get_RawOutY_Value()
        if(y >= self.minY and y <= self.maxY):
            return 0
        else:
            return y - self.meanY
            
    def Get_CalOutZ_Value(self):
        """Calibrated Z angular speed data"""
        z = self.Get_RawOutZ_Value()
        if(z >= self.minZ and z <= self.maxZ):
            return 0
        else:
            return z - self.meanZ
    
    def Get_CalOut_Value(self):
		"""Calibrated [X, Y, Z] value of angular speed, calibrated"""
		return [self.Get_CalOutX_Value(), self.Get_CalOutY_Value(), self.Get_CalOutZ_Value()]
		
        
    def Set_FifoThreshold_Value(self, value):
        self.__writeToRegister(self.__REG_RW_FIFO_CTRL_REG, self.__MASK_FIFO_CTRL_REG_WTM, value) 
    def Get_FifoThreshold_Value(self):
        """Fifo threshold - watermark level"""
        return self.__readFromRegister(self.__REG_RW_FIFO_CTRL_REG, self.__MASK_FIFO_CTRL_REG_WTM)
    
    def Set_FifoMode_Value(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_FIFO_CTRL_REG, self.__MASK_FIFO_CTRL_REG_FM, value, self.__FifoModeDict, 'FifoModeEnum') 
    def Get_FifoMode_Value(self):
        """Fifo mode"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_FIFO_CTRL_REG, self.__MASK_FIFO_CTRL_REG_FM, self.__FifoModeDict)

    def Get_FifoStoredDataLevel_Value(self):
        """Fifo stored data level"""
        return self.__readFromRegister(self.__REG_R_FIFO_SRC_REG, self.__MASK_FIFO_SRC_REG_FSS)
    
    def Get_IsFifoEmpty_Value(self):
        """Fifo empty"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_R_FIFO_SRC_REG, self.__MASK_FIFO_SRC_REG_EMPTY, self.__EnabledDict)
    
    def Get_IsFifoFull_Value(self):
        """Fifo full"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_R_FIFO_SRC_REG, self.__MASK_FIFO_SRC_REG_OVRN, self.__EnabledDict)
    
    def Get_IsFifoGreaterOrEqualThanWatermark_Value(self):
        """Fifo filling is greater or equal than watermark level"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_R_FIFO_SRC_REG, self.__MASK_FIFO_SRC_REG_WTM, self.__EnabledDict)

    def Set_Int1Combination_Value(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_ANDOR, value, self.__AndOrDict, 'AndOrEnum') 
    def Get_Int1Combination_Value(self):
        """Interrupt combination"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_ANDOR, self.__AndOrDict)
        
    def Set_Int1LatchRequest_Enabled(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_LIR, value, self.__EnabledDict, 'EnabledEnum') 
    def Get_Int1LatchRequest_Enabled(self):
        """Latch interrupt request"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_LIR, self.__EnabledDict)
    
    def Set_Int1GenerationOnZHigh_Enabled(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_ZHIE, value, self.__EnabledDict, 'EnabledEnum') 
    def Get_Int1GenerationOnZHigh_Enabled(self):
        """Int 1 generation on Z higher than threshold"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_ZHIE, self.__EnabledDict)
    
    def Set_Int1GenerationOnZLow_Enabled(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_ZLIE, value, self.__EnabledDict, 'EnabledEnum') 
    def Get_Int1GenerationOnZLow_Enabled(self):
        """Int 1 generation on Z lower than threshold"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_ZLIE, self.__EnabledDict)
    
    def Set_Int1GenerationOnYHigh_Enabled(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_YHIE, value, self.__EnabledDict, 'EnabledEnum') 
    def Get_Int1GenerationOnYHigh_Enabled(self):
        """Int 1 generation on Y higher than threshold"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_YHIE, self.__EnabledDict)
    
    def Set_Int1GenerationOnYLow_Enabled(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_YLIE, value, self.__EnabledDict, 'EnabledEnum') 
    def Get_Int1GenerationOnYLow_Enabled(self):
        """Int 1 generation on Y lower than threshold"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_YLIE, self.__EnabledDict)
    
    def Set_Int1GenerationOnXHigh_Enabled(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_XHIE, value, self.__EnabledDict, 'EnabledEnum') 
    def Get_Int1GenerationOnXHigh_Enabled(self):
        """Int 1 generation on X higher than threshold"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_XHIE, self.__EnabledDict)
    
    def Set_Int1GenerationOnXLow_Enabled(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_XLIE, value, self.__EnabledDict, 'EnabledEnum') 
    def Get_Int1GenerationOnXLow_Enabled(self):
        """Int 1 generation on X lower than threshold"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_INT1_CFG_REG, self.__MASK_INT1_CFG_XLIE, self.__EnabledDict)
    
    def Get_Int1Active_Value(self):
        """Int1 active"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_R_INT1_SRC_REG, self.__MASK_INT1_SRC_IA, self.__EnabledDict)
    
    def Get_ZHighEventOccured_Value(self):
        """Z high event occured"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_R_INT1_SRC_REG, self.__MASK_INT1_SRC_ZH, self.__EnabledDict)
    
    def Get_ZLowEventOccured_Value(self):
        """Z low event occured"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_R_INT1_SRC_REG, self.__MASK_INT1_SRC_ZL, self.__EnabledDict)
    
    def Get_YHighEventOccured_Value(self):
        """Y high event occured"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_R_INT1_SRC_REG, self.__MASK_INT1_SRC_YH, self.__EnabledDict)    
    
    def Get_YLowEventOccured_Value(self):
        """Y low event occured"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_R_INT1_SRC_REG, self.__MASK_INT1_SRC_YL, self.__EnabledDict)
    
    def Get_XHighEventOccured_Value(self):
        """X high event occured"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_R_INT1_SRC_REG, self.__MASK_INT1_SRC_XH, self.__EnabledDict)
    
    def Get_XLowEventOccured_Value(self):
        """X low event occured"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_R_INT1_SRC_REG, self.__MASK_INT1_SRC_XL, self.__EnabledDict)
    
    def Set_Int1ThresholdX_Value(self, value):
        self.__writeToRegister(self.__REG_RW_INT1_THS_XH, self.__MASK_INT1_THS_H, (value & 0x7f00) >> 8)
        self.__writeToRegister(self.__REG_RW_INT1_THS_XL, self.__MASK_INT1_THS_L, value & 0x00ff)
    def Set_Int1ThresholdY_Value(self, value):
        self.__writeToRegister(self.__REG_RW_INT1_THS_YH, self.__MASK_INT1_THS_H, (value & 0x7f00) >> 8)
        self.__writeToRegister(self.__REG_RW_INT1_THS_YL, self.__MASK_INT1_THS_L, value & 0x00ff)
    def Set_Int1ThresholdZ_Value(self, value):
        self.__writeToRegister(self.__REG_RW_INT1_THS_ZH, self.__MASK_INT1_THS_H, (value & 0x7f00) >> 8)
        self.__writeToRegister(self.__REG_RW_INT1_THS_ZL, self.__MASK_INT1_THS_L, value & 0x00ff)        
    def Get_Int1Threshold_Values(self):
        """(X,Y,Z) INT1 threshold value"""
        xh = self.__readFromRegister(self.__REG_RW_INT1_THS_XH, self.__MASK_INT1_THS_H)
        xl = self.__readFromRegister(self.__REG_RW_INT1_THS_XL, self.__MASK_INT1_THS_L)
        yh = self.__readFromRegister(self.__REG_RW_INT1_THS_YH, self.__MASK_INT1_THS_H)
        yl = self.__readFromRegister(self.__REG_RW_INT1_THS_YL, self.__MASK_INT1_THS_L)
        zh = self.__readFromRegister(self.__REG_RW_INT1_THS_ZH, self.__MASK_INT1_THS_H)
        zl = self.__readFromRegister(self.__REG_RW_INT1_THS_ZL, self.__MASK_INT1_THS_L)
        return (xh*256 + xl, yh*256 + yl, zh*256 + zl)
    
    def Set_Int1DurationWait_Enabled(self, value):
        self.__writeToRegisterWithDictionaryCheck(self.__REG_RW_INT1_DURATION, self.__MASK_INT1_DURATION_WAIT, value, self.__EnabledDict, 'EnabledEnum') 
    def Get_Int1DurationWait_Enabled(self):
        """Int 1 duration wait"""
        return self.__readFromRegisterWithDictionaryMatch(self.__REG_RW_INT1_DURATION, self.__MASK_INT1_DURATION_WAIT, self.__EnabledDict)
    
    def Set_Int1Duration_Value(self, value):
        self.__writeToRegister(self.__REG_RW_INT1_DURATION, self.__MASK_INT1_DURATION_D, value) 
    def Get_Int1Duration_Value(self):
        """Int 1 duration value"""
        return self.__readFromRegister(self.__REG_RW_INT1_DURATION, self.__MASK_INT1_DURATION_D)
