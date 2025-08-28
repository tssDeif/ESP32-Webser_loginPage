#pragma once

class LoginPage {
private:
    static const char* loginHTML;
    const char* username;
    const char* password;

public:
    LoginPage(const char* user = "admin", const char* pass = "password123");
    const char* getLoginPage() const;
    bool validateCredentials(const char* user, const char* pass) const;
};