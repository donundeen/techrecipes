# Setting up Wifi on the Arduino ESP32
a couple different approaches...


## Arduino ESP32 Access Point for Wifi Config

More documentation here: https://github.com/nrwiersma/ConfigManager

Tested on Arduino Feather Huzzah ESP32

With this code, if your ESP32 can't connect to the internet, it will create its own access point that you can connect to and then enter the network credentials for the WiFi network you want to connect to.
The Arduino will remember the connection details. You can save multiple credentials, if you're moving aroudn with the arduino to different networks. It will automatically connect to the available network

### Files
- [ap_connect.ino](ap_connect.ino) : the arduino coode
- [data/index.html](data/index.html) : the web page for collecting the data

### process for adiding the data file
(cribbed from https://github.com/nrwiersma/ConfigManager)

Upload the index.html file found in the data directory into the SPIFFS. Instructions on how to do this vary based on your IDE. Below are links instructions on the most common IDEs:

- Arduino IDE : https://github.com/me-no-dev/arduino-esp32fs-plugin
- Platform IO : https://docs.platformio.org/en/stable/platforms/espressif32.html#uploading-files-to-file-system-spiffs

## A more robust method, custom variables, and UDP implementation as well

This code shows how to set up an arduino
to create an Access Point and Capture page that presents a form
that can be used to configure the SSID and PW the Arduino will connect to

AND

create a form to configure arbitrary values that will be stored in the arduino's
non-volatile memory, to be loaded every time the arduino starts.
Super useful for makeing devices that you don't want to regrogram all the time,
just to change a few settings

AND

OSC over UDP implementation 

Requires that one pin (in this case A0) be attached to a button. (see resetButtonPin to change A0 to something else)
Pin needs to have built-in pullup resistor, or make your own pullup
hold down the button while turning arduino on, to activate the configuration page.
If the arduino starts and can't find wifi, it will also load the configuration page.

Note: right now, if the arduino can't find wifi after you've set it,
it won't save your custom config vars
Also, every time you save the new config vars,
you have to also set the wifi AP and PW to something that the arduino can actually connect to,
or it won't save the config vars.


CUSTOM CONFIG VARS:
Every place where you need to change code depending on what you Custom Config Vars are,
has been marked with // #CUSTOMCONFIGVARS

### Files
- [Arduino_AP_Config_also_UDP](Arduino_AP_Config_also_UDP) : the arduino code


## A simpler method

Just setting the connection details in the code itself

### Files
- [simple_connnect.ino](simple_connect.ino) : The arduino code
