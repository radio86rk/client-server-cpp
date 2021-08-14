#include <iostream>
#include "client.h"
#include "exceptions.h"
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctime>
#include <sys/time.h>

void pbf::Client::startClient()
{
    int sock;
    ssize_t send_len;
    char sec_buf[100];
    char msec_buf[10];
    sockaddr_in addr = {0};
    socklen_t addr_len = sizeof(sockaddr_in);
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(this->port);
	addr.sin_family = AF_INET;
	if ((sock = socket(AF_INET, SOCK_STREAM, NULL)) == SOCKET_ERROR) {
		throw pbf::InitSocketException(strerror(errno));
	}

    if (connect(sock,(const struct sockaddr*)&addr,addr_len) == SOCKET_ERROR) {
        throw pbf::ConnectSocketException(strerror(errno));
    }

    while (1)
    {
        timeval tv;

        gettimeofday(&tv,NULL);
        std::tm* tm_info = std::localtime(&tv.tv_sec);
        std::strftime(sec_buf, sizeof(sec_buf), "[%y-%m-%d %H:%M:%S", tm_info);
        std::snprintf(msec_buf,sizeof(msec_buf),".%03d] ",tv.tv_usec/1000);
        std::string message = sec_buf + std::string(msec_buf) + this->name + "\n";
        if((send_len = write(sock,message.c_str(),message.size())) != message.size())
            throw WriteDataSocketException(strerror(errno));
        sleep(this->delay);
    }
    
}

int
main(int argc,char **argv)
{
    if (argc != 4) {
        std::cerr<<"Missing <Name> <port> <delay> parameteres!"<<std::endl;
        return EXIT_FAILURE;
    }
    int port,delay;
    port = atoi(argv[2]);
    delay = atoi(argv[3]);
    pbf::Client client(argv[1],port,delay);
    try {
        client.startClient();
    }catch(pbf::BaseException e) {
        std::cerr<<e.getError()<<std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
    
}