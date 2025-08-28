#include "WebServerManager.h"
#include <WiFi.h>

WebServerManager* WebServerManager::instance = nullptr;

WebServerManager::WebServerManager(const char* wifi_ssid, const char* wifi_password)
    : server(80), ssid(wifi_ssid), password(wifi_password) {
    instance = this;
}

void WebServerManager::begin() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nConnected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, handleRootWrapper);
    server.on("/login", HTTP_POST, handleLoginWrapper);
    
    server.begin();
    Serial.println("HTTP server started");
}

void WebServerManager::handleClient() {
    server.handleClient();
}

WebServerManager* WebServerManager::getInstance() {
    return instance;
}

void WebServerManager::handleRoot() {
    server.send(200, "text/html", loginPage.getLoginPage());
}

void WebServerManager::handleLogin() {
    if (server.method() != HTTP_POST) {
        return server.send(405, "text/plain", "Method Not Allowed");
    }

    if (loginPage.validateCredentials(
        server.arg("username").c_str(), 
        server.arg("password").c_str())) {
        server.send(200, "text/html", "<h1>Login Successful!</h1>");
    } else {
        server.send(401, "text/html", "<h1>Login Failed!</h1>");
    }
}

void WebServerManager::handleRootWrapper() {
    if (instance) instance->handleRoot();
}

void WebServerManager::handleLoginWrapper() {
    if (instance) instance->handleLogin();
}