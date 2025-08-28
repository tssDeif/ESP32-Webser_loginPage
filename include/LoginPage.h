#pragma once

class LoginPage 
{
private:
    static const char* loginHTML;
    static const char* dashboardHTML;
    const char* username;
    const char* password;
    bool buttonState;
    const int ledPin = 2;

public:
    LoginPage(const char* user = "admin", const char* pass = "password123");
    const char* getLoginPage() const;
    const char* getDashboardPage() const;
    bool validateCredentials(const char* user, const char* pass) const;
    void toggleButton();
    bool getButtonState() const;
    void begin();
};