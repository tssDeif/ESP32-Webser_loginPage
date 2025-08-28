#include "LoginPage.h"
#include <cstring> 
#include <Arduino.h>
#include <WiFi.h>

const char* LoginPage::loginHTML = R"delimiter(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Login</title>
    <style>
        body { font-family: Arial; text-align: center; margin-top: 50px; }
        .login-form { display: inline-block; padding: 20px; border: 1px solid #ccc; }
        input, select { margin: 10px; padding: 5px; width: 200px; }
        .title { color: #333; margin-bottom: 30px; }
        .refresh-btn {
            background-color: #4CAF50;
            color: white;
            border: none;
            padding: 5px 10px;
            cursor: pointer;
            border-radius: 3px;
        }
    </style>
    <script>
        function refreshNetworks() {
            fetch('/scan-networks')
                .then(response => window.location.reload());
        }
    </script>
</head>
<body>
    <h1 class="title">ESP32 - DEIF Webserver</h1>
    <div class="login-form">
        <h2>WiFi Setup & Login</h2>
        <form action="/connect" method="POST">
            <select name="network" required>
                <option value="">Select WiFi Network</option>
                %NETWORK_LIST%
            </select><br>
            <input type="password" name="wifi_password" placeholder="WiFi Password" required><br>
            <input type="text" name="username" placeholder="Login Username" required><br>
            <input type="password" name="password" placeholder="Login Password" required><br>
            <button type="button" class="refresh-btn" onclick="refreshNetworks()">Refresh Networks</button><br>
            <input type="submit" value="Connect & Login">
        </form>
    </div>
</body>
</html>
)delimiter";

const char* LoginPage::dashboardHTML = R"delimiter(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Dashboard</title>
    <style>
        body { font-family: Arial; text-align: center; margin-top: 50px; }
        .dashboard { display: inline-block; padding: 20px; border: 1px solid #ccc; }
        .title { color: #333; margin-bottom: 30px; }
        .button {
            padding: 20px 40px;
            font-size: 24px;
            cursor: pointer;
            border: none;
            border-radius: 5px;
            margin: 20px;
        }
        .button.on {
            background-color: #4CAF50;
            color: white;
        }
        .button.off {
            background-color: #f44336;
            color: white;
        }
    </style>
    <script>
        function toggleButton() {
            fetch('/toggle', {method: 'POST'})
                .then(response => response.text())
                .then(state => {
                    const button = document.getElementById('toggleBtn');
                    if(state === 'ON') {
                        button.textContent = 'ON';
                        button.className = 'button on';
                    } else {
                        button.textContent = 'OFF';
                        button.className = 'button off';
                    }
                });
        }
    </script>
</head>
<body>
    <h1 class="title">ESP32 - DEIF Dashboard</h1>
    <div class="dashboard">
        <h2>Control Panel</h2>
        <button id="toggleBtn" onclick="toggleButton()" class="button %BUTTON_STATE%">%BUTTON_TEXT%</button>
    </div>
</body>
</html>
)delimiter";

LoginPage::LoginPage(const char* user, const char* pass) 
    : username(user), password(pass), buttonState(false), isAuthenticated(false) 
    {
    begin();
}

void LoginPage::begin() 
{
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
}

const char* LoginPage::getDashboardPage() const 
{
    static char buffer[4096];
    strcpy(buffer, dashboardHTML);
    
    const char* stateClass = buttonState ? "on" : "off";
    const char* stateText = buttonState ? "ON" : "OFF";
    
    char* pos = strstr(buffer, "%BUTTON_STATE%");
    if (pos) {
        memmove(pos + strlen(stateClass), pos + strlen("%BUTTON_STATE%"), 
                strlen(pos + strlen("%BUTTON_STATE%")) + 1);
        memcpy(pos, stateClass, strlen(stateClass));
    }
    
    pos = strstr(buffer, "%BUTTON_TEXT%");
    if (pos) {
        memmove(pos + strlen(stateText), pos + strlen("%BUTTON_TEXT%"), 
                strlen(pos + strlen("%BUTTON_TEXT%")) + 1);
        memcpy(pos, stateText, strlen(stateText));
    }
    
    return buffer;
}

bool LoginPage::validateCredentials(const char* user, const char* pass) const 
{
    if (!user || !pass) {
        return false;
    }
    return (strcmp(user, username) == 0 && strcmp(pass, password) == 0);
}

void LoginPage::toggleButton() 
{
    buttonState = !buttonState;
    digitalWrite(ledPin, buttonState ? HIGH : LOW);
}

bool LoginPage::getButtonState() const 
{
    return buttonState;
}

void LoginPage::scanNetworks() 
{
    networkList.clear();
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) 
    {
        networkList.push_back(WiFi.SSID(i));
    }
}

String LoginPage::getNetworksAsOptions() const 
{
    String options;
    for (const String& network : networkList) 
    {
        options += "<option value=\"" + network + "\">" + network + "</option>";
    }
    return options;
}

const char* LoginPage::getLoginPage() const 
{
    static char buffer[8192];
    String page = loginHTML;
    page.replace("%NETWORK_LIST%", getNetworksAsOptions());
    strcpy(buffer, page.c_str());
    return buffer;
}