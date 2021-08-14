#include "server.h"
#include "exceptions.h"
#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <thread>

const int MAX_RECV_BYTES = 4096;

pbf::Server::Server(short port, const char* filename)
{
    this->port = port;
    this->logFile.open(filename, std::ios::out | std::ios::app);

}

void pbf::Server::setPort(unsigned short port)
{
    this->port = port;
}

void pbf::Server::startServer()
{
    int sock,client_sock;
    sockaddr_in addr = {0};
    socklen_t addr_len = sizeof(sockaddr_in);
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(this->port);
	addr.sin_family = AF_INET;
	if ((sock = socket(AF_INET, SOCK_STREAM, NULL)) == SOCKET_ERROR) {
		throw pbf::InitSocketException(strerror(errno));
	}

	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		throw pbf::BindSocketException(strerror(errno));
	}

	if (listen(sock, SOMAXCONN) == SOCKET_ERROR){
		throw pbf::ListenSocketException(strerror(errno));
	}

    while(1) {
        if ((client_sock = accept(sock, (struct sockaddr*)&addr,&addr_len)) == SOCKET_ERROR){
		    throw pbf::AcceptSocketException(strerror(errno));
	    }
        std::thread t(&Server::requestHandle,this,client_sock);
        t.detach();
    }

}

void pbf::Server::requestHandle(int client_sock)
{
    char buffer[MAX_RECV_BYTES];
    while(1) {
        ssize_t recieveLen = read(client_sock,buffer,MAX_RECV_BYTES-1);
        if (recieveLen < 0)
            throw ReadDataSocketException(strerror(errno));
        if(!recieveLen)
            return;
        buffer[recieveLen] = 0;
        std::cout<<recieveLen<<" "<<buffer;
        this->mut.lock();
        this->logFile<<buffer;
        this->logFile.flush();
        this->mut.unlock();
    }

}

int
main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr<<"Missing port parameter!"<<std::endl;
        return EXIT_FAILURE;
    }
    int port = atoi(argv[1]);
    pbf::Server server(port,"log.txt");
    try {
        server.startServer();
    } catch (pbf::BaseException e) {
        std::cerr<<e.getError()<<std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;   
}