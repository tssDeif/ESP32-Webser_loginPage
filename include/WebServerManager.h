#pragma once

#include <WebServer.h>
#include "LoginPage.h"

class WebServerManager {
private:
    WebServer server;
    LoginPage loginPage;
    const char* ssid;
    const char* password;
    
    static WebServerManager* instance;
    void handleRoot();
    void handleLogin();

public:
    WebServerManager(const char* wifi_ssid, const char* wifi_password);
    void begin();
    void handleClient();
    static WebServerManager* getInstance();
    
    // Callback wrappers
    static void handleRootWrapper();
    static void handleLoginWrapper();
};