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


## A simpler method

Just setting the connection details in the code itself

### Files
- [simple_connnect.ino](simple_connect.ino) : The arduino code
