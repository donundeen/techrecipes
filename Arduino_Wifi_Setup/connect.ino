/*****************************************
 * Include Libraries
 ****************************************/
#include <ESP8266WiFi.h>
#include <ConfigManager.h>
#include <PubSubClient.h>

/****************************************
 * Define Constants
 ****************************************/
 
namespace{
  const char * AP_NAME = "Heybox Access Point"; // Assigns your Access Point name
}

String clientMac = "";
unsigned char mac[6];

struct Config {
  char name[20];
  bool enabled;
  int8 hour;
} config;

/****************************************
 * Initialize a global instance
 ****************************************/
WiFiClient espClient;
ConfigManager configManager;

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  /* Declare PINs as input/outpu */
  
  WiFi.macAddress(mac);
  clientMac += macToStr(mac);

  /* Access Point configuration */
  configManager.setAPName(AP_NAME);
  configManager.addParameter("name", config.name, 20);
  configManager.addParameter("enabled", &config.enabled);
  configManager.addParameter("hour", &config.hour);
  configManager.begin(config);

}

void loop() {  
  configManager.loop();    
  
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("not on wifi");
    Serial.println(WL_CONNECTED);
    Serial.println(WiFi.status());
    delay(500);
    return;    
  }else{
    Serial.println("connected to wifi");
  }
  
  delay(200);
}

/****************************************
 * Auxiliar Functions
 ****************************************/

String macToStr(const uint8_t* mac) {
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)result += ':';
  }
  Serial.println(result);
  return result;
}
