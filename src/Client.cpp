#include "Client.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Chat::Client::Client(int port, const char *address) : _port(port)
{
    _poll[0].fd = STDIN_FILENO;
    _poll[1].fd = -1;
    for (int i = 0; i < 2; ++i)
        _poll[i].events = POLLIN;

    addrinfo _request, *_response;
    bzero(&_request, sizeof(_request));
    _request.ai_family = AF_INET;
    _request.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(address, nullptr, &_request, &_response))
    {
        fprintf(stderr, "Unable to get address info.\n");
        exit(1);
    }

    _address = *(sockaddr_in *)_response->ai_addr;
    _address.sin_port = htons(port);
    freeaddrinfo(_response);
    
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1)
    {
        fprintf(stderr, "Unable to open socket.\n");
        exit(1);
    }
}

Chat::Client::~Client()
{
    if (_socket)
        close(_socket);
}

void Chat::Client::connect()
{
    if (::connect(_socket, (sockaddr *)&_address, sizeof(_address)) < 0)
    {
        fprintf(stderr, "Unable to connect.\n");
        exit(1);
    }

    _poll[1].fd = _socket;
}

void Chat::Client::loop()
{
    while (true)
    {
        char buffer[Chat::Client::SIZE];

        if (poll(_poll, 2, -1) < 0)
        {
            fprintf(stderr, "Error in poll.\n");
            break;
        }

        for (int i = 0; i < 2; ++i)
        {
            if (_poll[i].revents & POLLIN)
            {
                int size = read(_poll[i].fd, buffer, sizeof(buffer));
                if (size < 0)
                {
                    fprintf(stderr, "Unable to read from %d.\n", _poll[i].fd);
                    continue;
                }

                if (write(_poll[!i].fd, buffer, size) < 0)
                {
                    fprintf(stderr, "Unable to write to %d.\n", _poll[!i].fd);
                    continue;
                }
            }
        }
    }
}