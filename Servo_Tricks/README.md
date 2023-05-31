# Doing things with Servos on the Pi

`/usr/bin/python /home/pi/loginlog/servodial/servodial_runonce_rand.py`

**servodial_runonce_rand.py**

Note the servo pin is pin 13

```
import RPi.GPIO as GPIO
import time
from random import random

servoPIN = 13
GPIO.setmode(GPIO.BCM)
GPIO.setup(servoPIN, GPIO.OUT)

p = GPIO.PWM(servoPIN, 50) # GPIO 17 for PWM with 50Hz
p.start(2.5) # Initialization
p.ChangeDutyCycle(random() * 12.5)
time.sleep(1);
p.ChangeDutyCycle(random() * 12.5)
time.sleep(1);
p.ChangeDutyCycle(random() * 12.5)
time.sleep(1);
p.ChangeDutyCycle(random() * 12.5)
time.sleep(1);
p.stop()
GPIO.cleanup()
print("done")
```
