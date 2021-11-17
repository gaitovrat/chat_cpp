#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
#include <vector>

#include "Process.hpp"
#include "Semaphore.hpp"
#include "SharedMemory.hpp"

namespace Chat
{
    class Server
    {
    private:
        int _socket;
        sockaddr_in _address;
        pollfd _poll[2];
        std::vector<Chat::Process *> _processes;
        Semaphore _mutex;
        Semaphore _empty;
        Semaphore _full;
        SharedMemory _shm;

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
