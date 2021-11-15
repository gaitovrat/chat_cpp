#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <netinet/in.h>
#include <poll.h>

namespace Chat
{
    class Client
    {
    private:
        sockaddr_in _address;
        int _socket;
        pollfd _poll[2];

        static constexpr int SIZE = 1024;
    public:
        Client(int port, const char *address);
        ~Client();

        void connect();
        void loop();
    };
}
#endif