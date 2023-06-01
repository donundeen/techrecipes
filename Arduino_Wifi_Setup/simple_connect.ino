/*
 * Board types
 * ESP32 Arduino -> Adafruit ESP32 Feather
 * ESP32 Adruino -> ESP32 Dev Module
 */

/*#include <Arduino.h>
#include <WiFi.h>
*/
#include <WiFi.h>
// #include <ArduinoOSC.h> // you can use this if your borad supports only WiFi or Ethernet

WiFiClient espClient;
PubSubClient client(espClient);


// WiFi stuff
const char* ssid = "WIFI_SSID";
const char* pwd = "WIFI_PW";

// for having a consistent IP address
const IPAddress ip(10, 0, 0, 225);
//IPAddress ip;  // THIS device's IP  (need to tie to consistent mac addresses, with a table)
const IPAddress gateway(10, 0, 0, 1);
const IPAddress subnet(255, 255, 255, 0);

String thisarduinomac = "";

void setup() {

    Serial.begin(115200);
    
    thisarduinomac = WiFi.macAddress();
    Serial.print("this mac address is ");
    Serial.println(thisarduinomac);

    // WiFi stuff (no timeout setting for WiFi)
    Serial.print("connecting to SSID ");
    Serial.println(ssid);
   
    connect_wifi();
}




void connect_wifi(){
   if(WiFi.status() != WL_CONNECTED){
      Serial.println("connecting to wifi");
#ifdef ESP_PLATFORM
      WiFi.disconnect(true, true);  // disable wifi, erase ap info
      delay(1000);
      WiFi.mode(WIFI_STA);
#endif
  
      WiFi.begin(ssid, pwd);
      WiFi.config(ip, gateway, subnet); // this part only needed if you want a consistent IP address.
      
      while (WiFi.status() != WL_CONNECTED) {
          Serial.print(".");
          delay(500);
      }
      
      Serial.print("WiFi connected, IP = ");
      Serial.println(WiFi.localIP());
   }
  
}

void loop() {
  
    connect_wifi();


}

