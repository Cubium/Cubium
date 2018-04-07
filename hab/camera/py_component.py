import picamera
from time import time

DIR = "/home/Cubium_Pics/"

def takePicture():
    camera.capture(DIR + str(time()).replace(".", "") + '.jpg')
    return

def handleSpaData(data):
    if data == "take picture":
        takePicture()
        tookPic = True

def sendData():
    if tookPic:
        return "Took a Picture!"
        tookPic = False
    else:
        return ""

def init():
    camera = picamera.PiCamera()
    tookPic = False
