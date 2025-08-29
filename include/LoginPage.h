#pragma once
#include <vector>
#include <Arduino.h>

class LoginPage 
{
private:
    static const char* loginHTML;
    static const char* dashboardHTML;
    const char* username;
    const char* password;
    bool buttonState;
    bool button2State;
    const int ledPin = 2;
    const int led2Pin = 4;  // GPIO 4 (D4)
    std::vector<String> networkList;
    bool isAuthenticated;

public:
    LoginPage(const char* user = "admin", const char* pass = "password123");
    
    // Method declarations
    void begin();
    void scanNetworks();
    String getNetworksAsOptions() const;
    const char* getLoginPage() const;
    const char* getDashboardPage() const;
    bool validateCredentials(const char* user, const char* pass) const;
    void toggleButton();
    void toggleButton2();
    bool getButtonState() const;
    bool getButton2State() const;
    bool isUserAuthenticated() const 
    { 
        return isAuthenticated; 
    }
    void setAuthenticated(bool state) 
    { 
        isAuthenticated = state; 
    }
};