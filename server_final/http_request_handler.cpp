#include "http_request_handler.h"
#include "file_manager.h"
#include <sstream>
#include <iostream>
#include <cctype>


// Декодирование URL строк
std::string HttpRequestHandler::urlDecode(const std::string& str) {
    std::string result;
    result.reserve(str.size());
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '+') {
            result += ' ';
        }
        else if (str[i] == '%' && i + 2 < str.length()) {
            int value = 0;
            std::istringstream iss(str.substr(i + 1, 2));
            if (iss >> std::hex >> value) {
                result += static_cast<char>(value);
                i += 2;
            }
            else {
                result += '?'; // Invalid encoding
            }
        }
        else {
            result += str[i];
        }
    }
    return result;
}

// Обработка HTTP запросов
std::string HttpRequestHandler::handleRequest(const std::string& request) {
    if (request.find("POST /login") != std::string::npos) {
        size_t pos = request.find("\r\n\r\n");
        std::string body = request.substr(pos + 4);
        return handlePostLogin(body);
    }
    else {
        return serveLoginForm();
    }
}

// Обработка POST запроса для логина
std::string HttpRequestHandler::handlePostLogin(const std::string& body) {
    // Парсим тело запроса
    std::istringstream stream(body);
    std::string username, password;
    std::string part;

    while (std::getline(stream, part, '&')) {
        auto separator = part.find('=');
        if (separator != std::string::npos) {
            std::string key = part.substr(0, separator);
            std::string value = part.substr(separator + 1);
            std::cout << "Key: " << key << ", Value: " << value << std::endl;  // Логирование разбора
            if (key == "username") {
                username = value;
            }
            else if (key == "password") {
                password = value;
            }
        }
    }
    // Сохраняем данные в login_data.txt
    if (!username.empty() && !password.empty()) {
        FileManager::saveLoginData("login_data.txt", username, password);
        return "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nThank you for logging in.\r\n";
    }
    else {
        return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nInvalid login data.\r\n";
    }
}
// страница авторизации
std::string HttpRequestHandler::serveLoginForm() {
    return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
        "<html><body>"
        "<form action='/login' method='post'>"
        "Username: <input type='text' name='username'><br>"
        "Password: <input type='password' name='password'><br>"
        "<input type='submit' value='Login'>"
        "</form>"
        "</body></html>\r\n";
}
