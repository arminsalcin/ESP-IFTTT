#include <ESP8266WiFi.h>
#include <arduino.h>
#include "DataToMaker.h"
#include "DHT.h"

#define SERIAL_DEBUG // Uncomment this to dissable serial debugging

// define gpio pins here:
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

// pin for heatbeat LED
#define HEARTBEAT_PIN 13 //GPIO13


// Define program constants

const char* myKey = "your key"; // your maker key here
const char* ssid = "your ssid"; // your router ssid here
const char* password = "your password"; // your router password here
String projectName = "your project name here";

// declare new maker event with the name "ESP"
DataToMaker event(myKey, projectName);

//declare new dht event with name "dht"
DHT dht(DHTPIN, DHTTYPE);

// LEAVE SET
bool connectedToWiFI = false;

void setup()
{
#ifdef SERIAL_DEBUG
  Serial.begin(115200);
  delay(200);
  Serial.println();
#endif

  delay(10); // short delay
  pinMode(HEARTBEAT_PIN, OUTPUT);
  WiFi.mode(WIFI_STA);
  ConnectWifi();
  dht.begin();
}

void loop() {
  if (wifiConnected){
    
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  event.setValue(1,String(t));
  event.setValue(2,String(h));
  
      debugln("connecting...");
      if (event.connect()){
        debugln("Connected To Maker");
        event.post();
      }
      else debugln("Failed To Connect To Maker!");
    
    delay(1000); // pause for 1 second
    digitalWrite(HEARTBEAT_PIN, !digitalRead(HEARTBEAT_PIN));
  }
  else
  {
    delay(60 * 1000); // 1 minute delay before trying to re connect
    ConnectWifi();
  }
}

bool ConnectWifi()
{
  // Connect to WiFi network
  debugln();
  debugln();
  debug("Connecting to ");
  debugln(ssid);
  unsigned long startTime = millis();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED && startTime + 30 * 1000 >= millis()) {
    delay(500);
    debug(".");
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    debugln("");
    debugln("WiFi connected");
  }
  else
  {
    WiFi.disconnect();
    debugln("");
    debugln("WiFi Timed Out!");
  }
}

bool wifiConnected()
{
  return WiFi.status() == WL_CONNECTED;
}


void debug(String message)
{
#ifdef SERIAL_DEBUG
  Serial.print(message);
#endif
}

void debugln(String message)
{
#ifdef SERIAL_DEBUG
  Serial.println(message);
#endif
}

void debugln()
{
#ifdef SERIAL_DEBUG
  Serial.println();
#endif
}


