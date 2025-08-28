#include "LoginPage.h"
#include <cstring> 

const char* LoginPage::loginHTML = R"(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Login</title>
    <style>
        body { font-family: Arial; text-align: center; margin-top: 50px; }
        .login-form { display: inline-block; padding: 20px; border: 1px solid #ccc; }
        input { margin: 10px; padding: 5px; }
    </style>
</head>
<body>
    <h1 class="title"> ESP32 - Webserver </h1>
    <div class="login-form">
        <h2>Login</h2>
        <form action="/login" method="POST">
            <input type="text" name="username" placeholder="Username" required><br>
            <input type="password" name="password" placeholder="Password" required><br>
            <input type="submit" value="Login">
        </form>
    </div>
</body>
</html>
)";

LoginPage::LoginPage(const char* user, const char* pass) 
    : username(user), password(pass) {}

const char* LoginPage::getLoginPage() const 
{
    return loginHTML;
}

bool LoginPage::validateCredentials(const char* user, const char* pass) const 
{
    return (strcmp(user, username) == 0 && strcmp(pass, password) == 0);
}