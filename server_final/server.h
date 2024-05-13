#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>

class WebServer {
public:
    WebServer(const char* port, int backlog);
    ~WebServer();
    void run();

private:
    const char* port; // Номер порта, на котором сервер прослушивает входящие соединения
    int backlog; // Максимальное количество ожидающих соединений
    SOCKET listenSocket; // Сокет для прослушивания входящих соединений
    fd_set masterSet; // Набор дескрипторов сокетов для select
    SOCKET maxSocket; // Максимальный номер дескриптора сокета для использования в select

    void initializeWinsock();
    void createSocketAndBind();
    void listenForConnections();
    void handleNewConnections();
    void handleClientActivity(SOCKET socket);
    void closeSocket(SOCKET socket);
};

#endif
