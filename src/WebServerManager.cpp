#include "WebServerManager.h"
#include <WiFi.h>

WebServerManager* WebServerManager::instance = nullptr;

WebServerManager::WebServerManager() : server(80), isConnected(false) 
{
    instance = this;
}

bool WebServerManager::checkAuthentication() {
    if (!loginPage.isUserAuthenticated()) {
        server.sendHeader("Location", "/");
        server.send(303);  // See Other
        return false;
    }
    return true;
}

void WebServerManager::begin() {
    // Stop any existing WiFi connections
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);  // Set ESP32 as Access Point only initially
    
    // Create Access Point
    bool apSuccess = WiFi.softAP("ESP32-Setup", "12345678");  // SSID and password (min 8 chars)
    
    if (apSuccess) {
        IPAddress IP = WiFi.softAPIP();
        Serial.println("Access Point Started Successfully");
        Serial.print("AP IP address: ");
        Serial.println(IP);
    } else {
        Serial.println("Access Point Failed to Start");
    }

    // Delay to ensure AP is fully started
    delay(500);

    // Setup server routes
    server.on("/", HTTP_GET, handleRootWrapper);
    server.on("/connect", HTTP_POST, handleConnectWrapper);
    server.on("/login", HTTP_POST, handleLoginWrapper);
    server.on("/dashboard", HTTP_GET, handleDashboardWrapper);
    server.on("/toggle", HTTP_POST, handleToggleWrapper);
    server.on("/scan-networks", HTTP_GET, handleScanNetworksWrapper);
    
    // Start server
    server.begin();
    Serial.println("HTTP server started");

    // Initial network scan
    loginPage.scanNetworks();
}

void WebServerManager::handleScanNetworks() 
{
    loginPage.scanNetworks();
    server.send(200, "text/plain", "Networks scanned");
}

void WebServerManager::handleConnect() 
{
    String network = server.arg("network");
    String wifi_password = server.arg("wifi_password");
    String username = server.arg("username");
    String password = server.arg("password");

    // First validate the credentials
    if (!loginPage.validateCredentials(username.c_str(), password.c_str())) 
    {
        server.send(401, "text/html", 
            "<h1>Invalid Credentials</h1>"
            "<p>The provided username or password is incorrect</p>"
            "<p><a href='/'>Back to Login</a></p>");
        return;
    }

    if (network.length() > 0 && wifi_password.length() > 0) 
    {
        // Disconnect from any existing WiFi
        WiFi.disconnect(true);
        delay(1000);

        // Attempt to connect to the new network
        WiFi.begin(network.c_str(), wifi_password.c_str());
        
        Serial.print("Attempting to connect to: ");
        Serial.println(network);
        
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) 
        {
            delay(500);
            Serial.print(".");
            attempts++;
        }

        if (WiFi.status() == WL_CONNECTED) 
        {
            isConnected = true;
            loginPage.setAuthenticated(true);
            Serial.println("\nConnected successfully!");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
            
            server.send(200, "text/html", 
                "<h1>Connected Successfully!</h1>"
                "<p>Network: " + network + "</p>"
                "<p>IP Address: " + WiFi.localIP().toString() + "</p>"
                "<p><a href='/dashboard'>Go to Dashboard</a></p>");
        } 
        else 
        {
            isConnected = false;
            loginPage.setAuthenticated(false);
            Serial.println("\nConnection failed!");
            
            server.send(400, "text/html", 
                "<h1>Connection Failed</h1>"
                "<p>Could not connect to " + network + "</p>"
                "<p><a href='/'>Back to Setup</a></p>");
        }
    } 
    else 
    {
        server.send(400, "text/html", 
            "<h1>Invalid Parameters</h1>"
            "<p>Network name and password are required</p>"
            "<p><a href='/'>Back to Setup</a></p>");
    }
}

void WebServerManager::handleScanNetworksWrapper() 
{
    if (instance) instance->handleScanNetworks();
}

void WebServerManager::handleConnectWrapper() 
{
    if (instance) instance->handleConnect();
}

void WebServerManager::handleClient() 
{
    server.handleClient();
}

WebServerManager* WebServerManager::getInstance() 
{
    return instance;
}

void WebServerManager::handleRoot() 
{
    server.send(200, "text/html", loginPage.getLoginPage());
}

void WebServerManager::handleLogin() 
{
    if (server.method() != HTTP_POST) 
    {
        return server.send(405, "text/plain", "Method Not Allowed");
    }

    String username = server.arg("username");
    String password = server.arg("password");

    Serial.print("Login attempt - Username: ");
    Serial.println(username);

    // First, ensure we have both username and password
    if (username.length() == 0 || password.length() == 0) 
    {
        server.send(400, "text/html", 
            "<h1>Login Failed!</h1>"
            "<p>Both username and password are required</p>"
            "<p><a href='/'>Back to Login</a></p>");
        return;
    }

    // Validate credentials
    if (loginPage.validateCredentials(username.c_str(), password.c_str())) 
    {
        loginPage.setAuthenticated(true);
        server.sendHeader("Location", "/dashboard");
        server.send(303);
    } 
    else 
    {
        loginPage.setAuthenticated(false);
        server.send(401, "text/html", 
            "<h1>Login Failed!</h1>"
            "<p>Invalid username or password</p>"
            "<p><a href='/'>Back to Login</a></p>");
    }
}

void WebServerManager::handleDashboard() 
{
    if (!checkAuthentication()) 
    {
        return;
    }
    server.send(200, "text/html", loginPage.getDashboardPage());
}

void WebServerManager::handleToggle() 
{
    if (!checkAuthentication()) 
    {
        return;
    }
    loginPage.toggleButton();
    server.send(200, "text/plain", loginPage.getButtonState() ? "ON" : "OFF");
}

// Static wrapper functions
void WebServerManager::handleRootWrapper() 
{
    if (instance) instance->handleRoot();
}

void WebServerManager::handleLoginWrapper() 
{
    if (instance) instance->handleLogin();
}

void WebServerManager::handleDashboardWrapper() 
{
    if (instance) instance->handleDashboard();
}

void WebServerManager::handleToggleWrapper() 
{
    if (instance) instance->handleToggle();
}