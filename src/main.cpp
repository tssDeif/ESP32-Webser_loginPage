#include <Arduino.h>
#include "WebServerManager.h"

WebServerManager* webServer;

void setup() 
{
  Serial.begin(115200);
  delay(1000);  // Give serial connection time to start
    
  Serial.println("Starting ESP32 Web Server...");
    
  webServer = new WebServerManager();
  webServer->begin();
}

void loop() 
{
  webServer->handleClient();
  delay(2);
}