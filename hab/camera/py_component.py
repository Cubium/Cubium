import picamera

camera = picamera.PiCamera()
file_no = 0


def init():
    pass


# Take a picture
def handleSpaData():
    ++file_no

    filename = "/home/pi/CubiumPics/" + file_no + ".png"
    print("Taking picture: " + filename)

    camera.capture(filename)


# Return the filename of the latest picture
def sendData():
    return "/home/pi/CubiumPics/" + file_no + ".png"
