# To install Raspbian LITE OS on RPI 4/3/0

Use SD Card of at least 16GB

On Mac use Raspberry Pi Imager: https://www.raspberrypi.com/software/
- Download and install this app

## Run Raspberry Pi Imager:
- Choose OS-> Raspberry Pi OS (Other) ->Raspberry Pi OS Lite (32-bit)
- Choose Storage->[pick the SD card]

## Additional Settings:
- Pick the wifi login/pass from the keychain (probaby a good idea to do this, if you are going to be using this rapsberry pi in this environment with this wifi)
- Set Hostname: Raspberrypi.local (you can use another hostname if this pi is part of a bigger system where you want unique names. But usually you can leave this alone)
- Enable SSH->Check (leave other defaults)
### Set Username and password
- Username: pi (you can change this, but I recommend leaving the username pi, as almost all online examples will assume this username)
- password
###  Configure Wireless LAN
- Use your wifi name and PW
- Wireless LAN Country: CA if in canada
### Set Locale Settings: 
- Time Zone: select yours
- Keyboard layout: US

### UNSET "Eject media when finished"
because we want to do other stuff with the SD card before we eject it

### Finishing up with Raspberry Pi Imager
- Save
- Write (this takes a little bit of time)

# If using RPi 0, we need to set up SSH over USB:
- open terminal window on laptop
- right click on a file in the SD card, select "open iterm here"
- in boot folder, type `touch ssh` to create empty file called ssh
- edit config.txt, adding     `dtoverlay=dwc2` at the end, under [all]
- edit `cmdline.txt`. Look for `rootwait`, and add `modules-load=dwc2,g_ether` immediately after. (with a space after rootwait and after g_ether)

When using a Pi0, now you can plug a usb cable from your computer to the micro usb port closest to the middle of the pi, then `ssh pi@raspberrypi.local` from a terminal on your computer, to log in. 

### Eject the SD Card from your computer, remove it, and insert into the Raspberry Pi

# First Run on RPI
- Install SD card and turn on Pi by plugging it in
- When using a Pi0, plug a usb cable from your computer to the micro usb port closest to the middle of the pi

## Login with user/pass 
- `ssh pi@raspberrypi.local` from a terminal on your computer, to log in. 

## confirm wifi
`ping google.com` to confirm wifi working


 
