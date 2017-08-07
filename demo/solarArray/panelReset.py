import RPi.GPIO as GPIO
from time import sleep

def setAngle(angle):
    duty = angle / 18 + 2
    GPIO.output(18, True)
    pwm.ChangeDutyCycle(duty)
    sleep(1)
    GPIO.output(18, False)
    pwm.ChangeDutyCycle(0)
    
GPIO.setmode(GPIO.BOARD)
GPIO.setup(18,GPIO.OUT)
pwm=GPIO.PWM(18,50)
pwm.start(0)

setAngle(0)
pwm.stop()
GPIO.cleanup()
