#include <Arduino.h>
#include "WebServerManager.h"

// WiFi credentials
const char* ssid = "DEIF-Public";
const char* password = "DEIF1933";

WebServerManager* webServer;

void setup() 
{
    Serial.begin(115200);
    
    webServer = new WebServerManager(ssid, password);
    webServer->begin();
}

void loop() 
{
    webServer->handleClient();
}