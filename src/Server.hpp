#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>

namespace Chat
{
    class Server
    {
    private:
        int _socket;
        sockaddr_in _address;
        pollfd _poll[2];

        static constexpr int SIZE = 1024;
    public:
        Server(int port);
        ~Server();

        void bind();
        void listen();
        void loop();

    private:
        void communication();
    };
}

#endif