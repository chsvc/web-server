#include "server.h"

int main() {
    WebServer server("8000", 10);
    server.run();
    return 0;
}
