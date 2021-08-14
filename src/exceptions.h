#pragma once
#include <string>

namespace pbf {

const int SOCKET_ERROR = -1;

class BaseException {
    protected:
        std::string m_error;
        std::string m_reason;
    public:
        BaseException(char* err = "Something went wrong",char* reason = "?"):m_error(err),m_reason(reason){}
        std::string getError() { return (m_error + " : " + m_reason); }
};

class InitSocketException : public BaseException {
    public:
        InitSocketException(char *reason):BaseException("Error during create socket",reason){}
};

class BindSocketException : public BaseException {
    public:
        BindSocketException(char *reason):BaseException("Error during bind socket",reason){}
};

class ListenSocketException : public BaseException {
    public:
        ListenSocketException(char *reason):BaseException("Error during listen socket",reason){}
};

class AcceptSocketException : public BaseException {
    public:
        AcceptSocketException(char *reason):BaseException("Error during accept socket",reason){}
};

class ConnectSocketException : public BaseException {
    public:
        ConnectSocketException(char *reason):BaseException("Error during connect socket",reason){}
};

class ReadDataSocketException : public BaseException {
    public:
        ReadDataSocketException(char *reason):BaseException("Error during read data from socket",reason){}
};

class WriteDataSocketException : public BaseException {
    public:
        WriteDataSocketException(char *reason):BaseException("Error during write data to socket",reason){}
};

}
