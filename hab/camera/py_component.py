import picamera
from time import time

DIR = "/home/pi/Cubium_Pics/"

def takePicture():
    camera.capture(DIR + str(time()).replace(".", "") + '.jpg')
    return

def handleSpaData(data):
    if data == "take picture":
        takePicture()
        tookPic = True

def sendData():
    if tookPic:
        tookPic = False
        print('took a picture!');
        return "took a picture!"
    else:
        return ""

def init():
    camera = picamera.PiCamera()
    tookPic = False
