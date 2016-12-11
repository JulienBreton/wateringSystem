#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN 13
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE, 12);

WiFiServer server(80);
WiFiClient client;

const char* ssid = "";
const char* password = "";
float pfHum = 0.0f;
float pfTemp = 0.0f;

bool readRequest(WiFiClient& client) {
  
  bool currentLineIsBlank = true;
  
  while (client.connected()) {
    
    if (client.available()) {
      
      char c = client.read();
      
      if (c == '\n' && currentLineIsBlank) 
      {
        return true;
      } 
      else if (c == '\n')
      {
        currentLineIsBlank = true;
      }
      else if (c != '\r') 
      {
        currentLineIsBlank = false;
      }
    }
  }
  return false;
}

JsonObject& prepareResponse(JsonBuffer& jsonBuffer) {
  
  JsonObject& root = jsonBuffer.createObject();
  
  JsonArray& tempValues = root.createNestedArray("temperature");
    tempValues.add(pfTemp);
    
  JsonArray& humiValues = root.createNestedArray("humidity");
    humiValues.add(pfHum);
    
  return root;
}

void writeResponse(WiFiClient& client, JsonObject& json) {
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();

  json.prettyPrintTo(client);
}

void setup() {

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
 
  server.begin();

  //Serial.print("Use this URL to connect: ");
  //Serial.print("http://");
  //Serial.print(WiFi.localIP());
  //Serial.println("/");

   initDHT();
}

void loop() {
  
  WiFiClient client = server.available();
  
  if (client)
  {
    
    bool success = readRequest(client);
    
    if (success)
    {
         delay(1000);
         
         pfTemp = getTemperature();   
         pfHum = getHumidity();

        delay(500);
        
        StaticJsonBuffer<500> jsonBuffer;
        JsonObject& json = prepareResponse(jsonBuffer);
        
        writeResponse(client, json);
    }
    
    delay(1);
    
    client.stop();
  }
}

void initDHT(){

  dht.begin();
}

float getTemperature(){

  float t = dht.readTemperature();

  if (isnan(t)) {
    
    return pfTemp;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");

  return t;
}

float getHumidity(){

  float h = dht.readHumidity();

  if (isnan(h)) {
    
    return pfHum;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %\t");

  return h;
}

