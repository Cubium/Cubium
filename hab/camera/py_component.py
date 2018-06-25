import picamera

camera = picamera.PiCamera()
file_no = 0


def init():
    pass


# Take a picture
def handleSpaData():
    global file_no
    file_no = file_no + 1

    filename = fileName()
    print("Taking picture: " + filename)

    camera.capture(filename)


def fileName():
    return "/home/pi/CubiumPics/" + str(file_no) + ".png"


# Return the filename of the latest picture
def sendData():
    return fileName()
