#include "server.h"
#include <iostream>
#include "http_request_handler.h"

#pragma comment(lib, "Ws2_32.lib")
// Конструктор класса WebServer
WebServer::WebServer(const char* port, int backlog) : port(port), backlog(backlog), listenSocket(INVALID_SOCKET), maxSocket(0) {
    FD_ZERO(&masterSet);
}
// Деструктор класса WebServer
WebServer::~WebServer() {
    closesocket(listenSocket);
    WSACleanup();
}

// Инициализация Winsock
void WebServer::initializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        exit(1);
    }
}

// Создание сокета, его привязка к порту и настройка на прослушивание
void WebServer::createSocketAndBind() {
    struct addrinfo hints, * res;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, port, &hints, &res);

    listenSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
        freeaddrinfo(res);
        WSACleanup();
        exit(1);
    }

    bind(listenSocket, res->ai_addr, (int)res->ai_addrlen);
    freeaddrinfo(res);
}

// Начало прослушивания сокета
void WebServer::listenForConnections() {
    listen(listenSocket, backlog);
    FD_SET(listenSocket, &masterSet);
    maxSocket = listenSocket;
}

// Основной метод для запуска сервера
void WebServer::run() {
    initializeWinsock();
    createSocketAndBind();
    listenForConnections();

    std::cout << "Server is listening on port " << port << std::endl; // Сообщение в консоль, что сервер начал прослушивание на определенном порту.

    // Основной бесконечный цикл сервера, где он ожидает события на сокетах.
    while (true) {
        fd_set readSet = masterSet;

        // Функция select проверяет, произошло ли какое-либо событие на сокетах в readSet
        // Она блокирует выполнение программы до тех пор, пока не произойдет событие или не закончится таймаут
        if (select(maxSocket + 1, &readSet, NULL, NULL, NULL) == SOCKET_ERROR) {
            std::cerr << "Select error " << WSAGetLastError() << std::endl;
            break;
        }

        for (int i = 0; i <= maxSocket; ++i) {
            if (FD_ISSET(i, &readSet)) {
                if (i == listenSocket) {
                    handleNewConnections();
                }
                else {
                    handleClientActivity(i);
                }
            }
        }
    }
}

// Обработка новых соединений
void WebServer::handleNewConnections() {
    SOCKET clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
    }
    else {
        FD_SET(clientSocket, &masterSet);
        if (clientSocket > maxSocket) {
            maxSocket = clientSocket;
        }
        std::cout << "New connection " << clientSocket << std::endl;
    }
}

// Обработка активности клиента
void WebServer::handleClientActivity(SOCKET socket) {
    char buffer[1024];
    // Принимаем на один символ меньше, чтобы оставить место для нуль-терминатора
    int bytesRead = recv(socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) {
        closeSocket(socket);
    }
    else {
        // Устанавливаем нуль-терминатор в конец полученных данных
        buffer[bytesRead] = '\0';
        std::string request(buffer);
        std::string response = HttpRequestHandler::handleRequest(request);
        send(socket, response.c_str(), static_cast<int>(response.length()), 0);
    }
}


// Закрытие сокета и удаление его из мастер-сета
void WebServer::closeSocket(SOCKET socket) {
    closesocket(socket);
    FD_CLR(socket, &masterSet);
    std::cout << "Connection closed " << socket << std::endl;
}
