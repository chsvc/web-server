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
    const char* port; // ����� �����, �� ������� ������ ������������ �������� ����������
    int backlog; // ������������ ���������� ��������� ����������
    SOCKET listenSocket; // ����� ��� ������������� �������� ����������
    fd_set masterSet; // ����� ������������ ������� ��� select
    SOCKET maxSocket; // ������������ ����� ����������� ������ ��� ������������� � select

    void initializeWinsock();
    void createSocketAndBind();
    void listenForConnections();
    void handleNewConnections();
    void handleClientActivity(SOCKET socket);
    void closeSocket(SOCKET socket);
};

#endif
