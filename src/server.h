#pragma once
#include <fstream>
#include <mutex>

namespace pbf {
    
class Server {
    unsigned short port;
    std::ofstream logFile;
    std::mutex mut;
public:
    Server(short port, const char* filename);
    Server(){};
    ~Server(){};
    void startServer();
    void setPort(unsigned short port);
    unsigned short getPort(){return this->port;}
    void requestHandle(int client_sock);
};

}