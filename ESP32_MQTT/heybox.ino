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
  const char * MQTT_SERVER = "io.adafruit.com"; 
  const char * TOKEN = ""; // Assigns your adafruit TOKEN
//  const char * USERNAME = "gumakerhub";
//  const char * FEEDNAME = "makerhubevents.backdoorbell";
  const char * USERNAME = "donundeen";
  const char * FEEDNAME_TO = "heybox-todon";
  const char * FEEDNAME_FROM = "heybox-toliz";

  int LED = 16;
  int BUTTONPIN = 2;
}

char topic_to[150];
char topic_from[150];
char payload[50];
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
PubSubClient client(espClient);
ConfigManager configManager;


/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length){
  String input = String((char *)payload).substring(0, length);

  if(input == "ON"){
    digitalWrite(LED, LOW);
  }else{
    digitalWrite(LED, HIGH);    
  }
}

void reconnect() {
  
  while (!client.connected()) {
//    configManager.loop();    
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    Serial.println(clientMac.c_str());
//    if (client.connect(clientMac.c_str(), TOKEN, NULL)) {
    if (client.connect(clientMac.c_str(), USERNAME, TOKEN)) {
      Serial.println("connected");
      client.subscribe(topic_from);      
      break;
    } else {
//        configManager.reset();
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");
      for(uint8_t Blink=0; Blink<=3; Blink++){
        digitalWrite(LED, LOW);
        delay(500);
        digitalWrite(LED, HIGH);
        delay(500);
      }
    }
  }
}

String macToStr(const uint8_t* mac) {
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)result += ':';
  }
  Serial.println(result);
  return result;
}

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  /* Declare PINs as input/outpu */
  pinMode(BUTTONPIN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  
  /* Assign WiFi MAC address as MQTT client name */
  WiFi.macAddress(mac);
  clientMac += macToStr(mac);

  /* Access Point configuration */
  configManager.setAPName(AP_NAME);
  configManager.addParameter("name", config.name, 20);
  configManager.addParameter("enabled", &config.enabled);
  configManager.addParameter("hour", &config.hour);
  configManager.begin(config);

  /* Set Sets the server details */
  
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
  
  /* Build the topic request */
  sprintf(topic_to, "%s/f/%s", USERNAME, FEEDNAME_TO);
  sprintf(topic_from, "%s/f/%s", USERNAME, FEEDNAME_FROM);
}




int prevVal = 0;
void loop() {  
  configManager.loop();    
  
  /* MQTT client reconnection */
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("not on wifi");
    Serial.println(WL_CONNECTED);
    Serial.println(WiFi.status());
    delay(500);
    return;    
  }else{

  }
  
  if (!client.connected()) {
    Serial.println("not connected, recon");
      reconnect();
  }

  int button = digitalRead(BUTTONPIN);
  if(button == 0){
    button = 1;
  }else{
    button = 0;  
  }
  Serial.println(button);

  if(button != prevVal){
    // new value;
    Serial.println("sending");
    Serial.println(button);
    if(button == 0){
      client.publish(topic_to, "OFF"); // turn it off for now...       
    }else{
      client.publish(topic_to, "ON"); // turn it off for now...       
    }
  }
  prevVal = button;
  
  client.loop();
  delay(200);
  
}
