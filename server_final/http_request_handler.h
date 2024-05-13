#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H

#include <string>
#include "file_manager.h"

class HttpRequestHandler {
    /*класс для обработки HTTP запроса*/
public:
    static std::string handleRequest(const std::string& request);  // Обработка запроса
    static std::string urlDecode(const std::string& str); // Декодирует URL для корректного отображения логина и пароля в файле

private:
    static std::string handlePostLogin(const std::string& body); // Извлекает введенные данные  
    static std::string serveLoginForm(); // Возвращает HTML-форму
};

#endif 
