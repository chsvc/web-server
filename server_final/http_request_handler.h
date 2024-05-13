#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H

#include <string>
#include "file_manager.h"

class HttpRequestHandler {
    /*����� ��� ��������� HTTP �������*/
public:
    static std::string handleRequest(const std::string& request);  // ��������� �������
    static std::string urlDecode(const std::string& str); // ���������� URL ��� ����������� ����������� ������ � ������ � �����

private:
    static std::string handlePostLogin(const std::string& body); // ��������� ��������� ������  
    static std::string serveLoginForm(); // ���������� HTML-�����
};

#endif 
