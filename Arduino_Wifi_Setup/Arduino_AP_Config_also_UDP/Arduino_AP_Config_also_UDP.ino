/*
 * Libraries to install (include dependencies)
 * OSC
 * 
 */

/*
This code show how to set up an arduino 
to create an Access Point and Capture page that presents a form 
that can be used to configure the SSID and PW the Arduino will connect to
AND
create a form to configure arbitrary values that will be stored in the arduino's
non-volatile memory, to be loaded every time the arduino starts.
Super useful for makeing devices that you don't want to regrogram all the time,
just to change a few settings

Requires that one pin (in this case A0) be attached to a button. (see resetButtonPin to change A0 to something else)
Pin needs to hve built-in pullup resistor, or make your own pullup
hold down the button while turning arduino on, to activate the configuration page.
If the arduino starts and can't find wifi, it will also load the configuration page.

Note: right now, if the arduino can't find wifi after you've set it,
it won't save your custom config vars
Also, every time you save the new config vars, 
you have to also set the wifi AP and PW to something that the arduino can actually connect to,
or it won't save the config vars.

CUSTOM CONFIG VARS:
#CUSTOMCONFIGVARS
Every place where you need to change code depending on what you Custom Config Vars are,
has been marked with // #CUSTOMCONFIGVARS

*/


// NETWORK_LIBS
/*
 * Required libraries to install in the arduino IDE (use the Library Manager to find and install):
 * https://github.com/bblanchon/ArduinoJson : ArduinoJson
 * https://github.com/CNMAT/OSC : OSC
 * https://github.com/tzapu/WiFiManager  : WiFiManager

 */
// this is all the OSC libraries
#include <SLIPEncodedSerial.h>
#include <OSCData.h>
#include <OSCBundle.h>
#include <OSCBoards.h>
#include <OSCTiming.h>
#include <OSCMessage.h>
#include <OSCMatch.h>
// these the libraries for connecting to WiFi
#include <WiFi.h>

// CONFIG WEBPAGE INCLUDES
#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#ifdef ESP32
  #include <SPIFFS.h>
#endif
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson


/// NETWORK CONFIGS  
// WIFI the arduino connects to . This wll get changed when you configure it over it's own AP/webpage.
const char *WIFI_SSID = "some_default_ssid_to_be_replaced_later";
const char *WIFI_PASSWORD = "wifipassword";

char *UDPReceiverIP = "192.168.0.200"; // ip where UDP messages are going. Configured via the arduino's AP/webpage
int UDPPort = 7002; // the UDP port that Max is listening on. Configured via the arduino's AP/webpage

// NETWORK+SENSOR UDP CONFIGS. Configured via the arduino's AP/webpage
const char *DEVICE_NAME = "CHANGE_ME"; // this is part of teh UDP channel name, AND the name of the SSID

const char *DEVICE_ID_SUFFIX = "/val"; // this part hard-coded here, not configured
char DEVICE_ID[40] = "/"; // the DEVICE_ID is the combo of the unique DEVICE and the DEVICE_ID_SUFFIX

// NO NETWORK MODE? for testing sensor without network
const bool no_network = false;

////////////////////////////////////////////
// NETWORK SPECIFIC CODE - SHOULDN'T CHANGE
/* 
 *  WIFI_MODE_ON set to true to send osc data over WIFI.
 *  When this is true: 
 *  -- if the arduino can't connect to wifi, it will create its own AP, named esp32_ap (pw 12345678)
 *  -- you'll need to connect to that SSID via your phone, and use the interface that pops up on your phone 
 *     to configure the SSID and PW of the router you want to connect to
 *  When WIFI_MODE_ON = false, you need the arduino connected to the laptop, 
 *  and it will send data over serial USB
 */
const boolean WIFI_MODE_ON = true;
/* 
    if we aren't using the auto-configuration process, 
    and we want to hard-code the router's SSID and password here.
    Also set HARDCODE_SSID = true
*/
const boolean HARDCODE_SSID = false;
// remember you can't connect to 5G networks with the arduino. 
bool wifi_connected =false;
/*
 * Sometimes we need to delete the SSIDs that are stored in the config of the arduino.
 * Set this value to TRUE and rerun the arduino, to remove all the stored SSIDs 
 * (aka clear the configuration storage). 
 * Then set it badk to false to start saving new SSID/Passwords
 * 
 */
const boolean DELETE_SSIDS = false;
String thisarduinomac = "";
String thisarduinoip = "";
//create UDP instance
WiFiUDP udp;
OSCErrorCode error;

// wifi autoconnect code
// CONFIG WEBPAGE PINS AND VARS
int resetButtonPin = A0;

/* 
. Configured via the arduino's AP/webpage
These values are used in the webpage/config code,
and/or retrieved from the stored config.json file, 
and set the values for:
UDPReceiverIP 
UDPPort 
DEVICE_NAME
 An artifact of how I cobbled this code together, to be cleaned up later. 
*/
char icanmusic_server_ip[40]; // #CUSTOMCONFIGVARS
char icanmusic_port[6] = "7002"; // #CUSTOMCONFIGVARS
char this_device_name[34] = "RENAME_ME";  // #CUSTOMCONFIGVARS

//flag for saving data
bool shouldSaveConfig = false;


// END NETWORK-SPECIFIC VARS
//////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
// SENSOR code
void device_setup(){
  // any device-specific setup code goes here
}

void device_loop(){
  // any device-specific loop code goes here
  // usually including a call to sendOSCUDP
}

// NETWORK+SENSOR CODE
// sending data over OSC/UDP.
// the input values of this function may vary,
// depending on the number of values to send to the OSC/UDP receiver

void sendOSCUDP(int sensorVal1){
 if(WiFi.status() == WL_CONNECTED){   
  //send hello world to server
  char ipbuffer[20]; // this holds this arduino's IP address, to send along with the message
  thisarduinoip.toCharArray(ipbuffer, 20);
  OSCMessage oscmsg(DEVICE_ID);  
  oscmsg.add(sensorVal1).add(ipbuffer);
  udp.beginPacket(UDPReceiverIP, UDPPort);
//  udp.write(buffer, msg.length()+1);
  oscmsg.send(udp);
  udp.endPacket();
  oscmsg.empty();
 }else{
  Serial.println("not sending udp, not connected");
 }

}





///////////////////////////////////////////////////
// BELOW HERE  SHOULD BE THE SAME FOR ALL DEVICES
void setup(){
  Serial.begin(9600);
  pinMode(BUILTIN_LED, OUTPUT); // setting up LED for message flashing
  if(!no_network){  
    network_setup();
  }
  device_setup();
}


void loop(){
  if(!no_network){
    network_loop();
  }
  device_loop();
}

void network_setup() {
  // the DEVICE_ID is for OSC/UDP things
  strcat(DEVICE_ID, DEVICE_NAME);
  strcat(DEVICE_ID, DEVICE_ID_SUFFIX);
  Serial.print("DEVICE_ID ");
  Serial.println(DEVICE_ID);
  delay(1000);
  Serial.println("setup");

  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  thisarduinomac = WiFi.macAddress();

  if(WIFI_MODE_ON){
      // wifi config business
    if(HARDCODE_SSID){
      Serial.println("connecting to hardcoded SSID");
      Serial.println(WIFI_SSID);
      Serial.println(WIFI_PASSWORD);      
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        // wifi status codes: https://realglitch.com/2018/07/arduino-wifi-status-codes/
        delay(1000);
        Serial.print(".");
        Serial.print(WiFi.status());
        Serial.print(WL_CONNECTED);
      }
    }else{
      config_webpage_setup();
    }
  }
}



void network_loop(){
  configUdp();
}

// this function will delete the existing config.json file.
// run this when you add/remove/change type of config variables.
// otherwise you'll crash the arduino and it will go into a reset loop.
void deleteAllCredentials(void) {
  Serial.println("deleting all stored SSID credentials");
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }  
  SPIFFS.remove("/config.json");
}



/*
 * connecting to UDP port on recipient machine
 */
void configUdp(){
  if(WIFI_MODE_ON){
    if(!wifi_connected && WiFi.status() == WL_CONNECTED){
      Serial.println("HTTP server:" + WiFi.localIP().toString());
      thisarduinoip = WiFi.localIP().toString();
      Serial.println("SSID:" + WiFi.SSID());
      wifi_connected = true;
      udp.begin(UDPPort);
    }
    if(WiFi.status() != WL_CONNECTED){
      Serial.println("wifi not connected");
      wifi_connected = false;
    }
  }
}


////////////////// SETING UP CONFIG WEBPAGE - FOR WIFI AND OTHER VALUES
//define your default values here, if there are different values in config.json, they are overwritten.
// My values: (in addition to WIFI data)
// icanmusic_server_ip
// icanmusic_port
// this_device_name

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}


// runs at setup time
void config_webpage_setup() {
  pinMode(resetButtonPin, INPUT_PULLUP);
  bool configMode = false;
  if(digitalRead(resetButtonPin) == LOW){
    Serial.println("config Mode ON");
    digitalWrite(BUILTIN_LED, HIGH);
    configMode = true;
  }else{
    digitalWrite(BUILTIN_LED, LOW);
  }

  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);

 #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
        DynamicJsonDocument json(1024);
        auto deserializeError = deserializeJson(json, buf.get());
        serializeJson(json, Serial);
        if ( ! deserializeError ) {
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
#endif
          Serial.println("\nparsed json");
          strcpy(icanmusic_server_ip, json["icanmusic_server_ip"]);  // #CUSTOMCONFIGVARS
          strcpy(icanmusic_port, json["icanmusic_port"]);  // #CUSTOMCONFIGVARS
          strcpy(this_device_name, json["this_device_name"]);  // #CUSTOMCONFIGVARS
        } else {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_icanmusic_server_ip("server", "iCanMusic server IP", icanmusic_server_ip, 40);  // #CUSTOMCONFIGVARS
  WiFiManagerParameter custom_icanmusic_port("port", "ICanMusic port", icanmusic_port, 6);  // #CUSTOMCONFIGVARS
  WiFiManagerParameter custom_this_device_name("devicename", "Device Name", this_device_name, 32);  // #CUSTOMCONFIGVARS

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip

  //add all your parameters here
  wifiManager.addParameter(&custom_icanmusic_server_ip);  // #CUSTOMCONFIGVARS
  wifiManager.addParameter(&custom_icanmusic_port);  // #CUSTOMCONFIGVARS
  wifiManager.addParameter(&custom_this_device_name);  // #CUSTOMCONFIGVARS

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimum quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);
  char* apname = this_device_name;
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //and goes into a blocking loop awaiting configuration
  //  if (!wifiManager.autoConnect(apname, "password")) {
  // instead we fire this up if the button is pressed at startup
  wifiManager.setConfigPortalTimeout(120);

  if(configMode){
    if(!wifiManager.startConfigPortal(apname)){
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }
  }else{
    if(!wifiManager.autoConnect(apname)){
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }
  }
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  digiflash(BUILTIN_LED, 10, 100, LOW);

  //read updated parameters
  strcpy(icanmusic_server_ip, custom_icanmusic_server_ip.getValue());  // #CUSTOMCONFIGVARS
  strcpy(icanmusic_port, custom_icanmusic_port.getValue());  // #CUSTOMCONFIGVARS
  strcpy(this_device_name, custom_this_device_name.getValue());  // #CUSTOMCONFIGVARS
  Serial.println("The values in the file are: ");
  Serial.println("\ticanmusic_server_ip : " + String(icanmusic_server_ip));  // #CUSTOMCONFIGVARS
  Serial.println("\ticanmusic_port : " + String(icanmusic_port));  // #CUSTOMCONFIGVARS
  Serial.println("\tthis_device_name : " + String(this_device_name));  // #CUSTOMCONFIGVARS

  UDPReceiverIP = icanmusic_server_ip; // ip where UDP messages are going
  UDPPort = atoi(icanmusic_port); // convert to int //  7002; // the UDP port that Max is listening on
  DEVICE_NAME = this_device_name;
  strcpy(DEVICE_ID, "/");
  strcat(DEVICE_ID, DEVICE_NAME);
  strcat(DEVICE_ID, DEVICE_ID_SUFFIX);

  Serial.print("\t UDPPort ");
  Serial.println(UDPPort);
  Serial.println("\tDEVICE_ID : " + String(DEVICE_ID));

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
#if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
    DynamicJsonDocument json(1024);
#else
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
#endif
    json["icanmusic_server_ip"] = icanmusic_server_ip;  // #CUSTOMCONFIGVARS
    json["icanmusic_port"] = icanmusic_port;  // #CUSTOMCONFIGVARS
    json["this_device_name"] = this_device_name;  // #CUSTOMCONFIGVARS

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

#if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
    serializeJson(json, Serial);
    serializeJson(json, configFile);
#else
    json.printTo(Serial);
    json.printTo(configFile);
#endif
    configFile.close();
    digiflash(BUILTIN_LED, 4, 250, LOW);
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
}


// function for sending a specific number of flashes of LED. 
// Useful for different types of alert messages
void digiflash(int pin, int numflash, int delaytime, int endval){
  for(int i = 0; i< numflash; i++){
    digitalWrite(pin, HIGH);
    delay(delaytime);
    digitalWrite(pin, LOW);
    delay(delaytime);
  }
  digitalWrite(pin, endval);
}

