#pragma once
#include <string>

namespace pbf {
    class Client {
        int delay;
        unsigned short port;
        std::string name;

        public:
            Client(const std::string name,int port, int delay):name(name),port(port),delay(delay){}
            ~Client(){}
            void startClient();
    };
}