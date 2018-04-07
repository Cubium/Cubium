import picamera
from time import time

DIR = "/home/pi/Cubium_Pics/"
camera = picamera.PiCamera()
tookPic = False

def takePicture():
    camera.capture(DIR + str(time()).replace(".", "") + '.jpg')

def handleSpaData():
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
    pass
