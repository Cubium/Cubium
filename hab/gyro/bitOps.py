#!/usr/bin/python3

def CheckBit(value, position):
    mask = 1 << position
    return value & mask == mask

def SetBit(value, position):
    return value | (1 << position)

def ClearBit(value, position):
    return value & ~(1 << position)

def FlipBit(value, position):
    return value ^ (1 << position)
    
    
    
def CheckBits(value, mask):
    return value & mask == mask

def SetBits(value, mask):
    return value | mask

def ClearBits(value, mask):
    return value & (~mask)

def FlipBits(value, mask):
    return value ^ mask

def SetValueUnderMask(valueToSetUnderMask, currentValue, mask):
    currentValueCleared = ClearBits(currentValue, mask) # clear bits under mask
    i = 0
    while (mask % 2 == 0 and mask != 0x00):
        mask = mask >> 1
        i += 1
    return SetBits(valueToSetUnderMask << i, currentValueCleared)

def GetValueUnderMask(currentValue, mask):
    currentValueCleared = ClearBits(currentValue, ~mask) # clear bits not under mask
    i = 0
    while (mask % 2 == 0 and mask != 0x00):
        mask = mask >> 1
        i += 1
    return currentValueCleared >> i

def TwosComplementToByte(value):
    if value >= 0 and value <= 127:
        return value
    else:
        return value - 256
    
def TwosComplementToCustom(value, signBitPosition):
    if value >= 0 and value <= (1<<signBitPosition)-1:
        return value
    else:
        return value - (2 << signBitPosition)