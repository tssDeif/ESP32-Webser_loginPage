#pragma once
#include <WebServer.h>
#include "LoginPage.h"

class WebServerManager 
{
private:
    WebServer server;
    LoginPage loginPage;
    bool isConnected;
    static WebServerManager* instance;

    bool checkAuthentication();
    void handleRoot();
    void handleLogin();
    void handleDashboard();
    void handleToggle();
    void handleToggle2();
    void handleScanNetworks();
    void handleConnect();

public:
    WebServerManager();
    void begin();
    void handleClient();
    static WebServerManager* getInstance();
    
    static void handleRootWrapper();
    static void handleLoginWrapper();
    static void handleDashboardWrapper();
    static void handleToggleWrapper();
    static void handleToggle2Wrapper();
    static void handleScanNetworksWrapper();
    static void handleConnectWrapper();
};