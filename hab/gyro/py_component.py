from L3GD20 import L3GD20
import time

# Communication object
s = L3GD20(busId = 1, slaveAddr = 0x6b, ifLog = False, ifWriteBlock=False)

def init():
    # Preconfiguration
    s.Set_PowerMode("Normal")
    s.Set_FullScale_Value("250dps")
    s.Set_AxisX_Enabled(True)
    s.Set_AxisY_Enabled(True)
    s.Set_AxisZ_Enabled(True)

    # Print current configuration
    s.Init()
    s.Calibrate()

def handleSpaData():
    pass

def sendData():
    # Calculate angle
    dt = 0.02
    x = 0
    y = 0
    z = 0
    time.sleep(dt)
    dxyz = s.Get_CalOut_Value()
    x += dxyz[0]*dt;
    y += dxyz[1]*dt;
    z += dxyz[2]*dt;
    return "Gyro: X:{:.2f}, Y:{:.2f}, Z:{:.2f}".format(x, y, z)
