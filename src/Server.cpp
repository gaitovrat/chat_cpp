#include "Server.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Chat::Server::Server(int port) : _socket(0)
{
    _poll[0].fd = STDIN_FILENO;
    _poll[1].fd = -1;
    for (int i = 0; i < 2; ++i)
        _poll[i].events = POLLIN;

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1)
    {
        fprintf(stderr, "Unable to open socket.\n");
        exit(1);
    }

    int options = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(options)) < 0)
    {
        fprintf(stderr, "Unable to set options.\n");
        exit(1);
    }

    _address.sin_family = AF_INET;
    _address.sin_port = htons(port);
    _address.sin_addr = {INADDR_ANY};
}

Chat::Server::~Server()
{
    close(_socket);
}

void Chat::Server::bind()
{
    if (::bind(_socket, (const sockaddr *)&_address, sizeof(_address)) < 0)
    {
        fprintf(stderr, "Unable to bind socket.\n");
        exit(1);
    }
}

void Chat::Server::listen()
{
    if (::listen(_socket, 1) < 0)
    {
        fprintf(stderr, "Unable to listen socket.\n");
        exit(1);
    }
}

void Chat::Server::loop()
{
    while (true)
    {
        int client = -1;
        _poll[1].fd = _socket;

        while (true)
        {
            if (poll(&_poll[1], 1, -1) < 0)
            {
                fprintf(stderr, "Unable to poll.\n");
                return;
            }

            if (_poll[1].revents & POLLIN)
            {
                sockaddr_in client_address;
                int size = sizeof(client_address);

                client = accept(_poll[1].fd, (sockaddr *)&client_address, (socklen_t *)&size);
                if (client == -1)
                {
                    fprintf(stderr, "Unable to connect client.\n");
                    return;
                }

                break;
            }
        }

        _poll[1].fd = client;
        communication();
    }
}

void Chat::Server::communication()
{
    while (true)
    {
        char buffer[Chat::Server::SIZE];
        if (poll(_poll, 2, -1) < 0)
        {
            fprintf(stderr, "Unable to poll 2.\n");
            return;
        }

        for (int i = 0; i < 2; ++i)
        {
            if (_poll[i].revents & POLLIN)
            {
                int size = read(_poll[i].fd, buffer, sizeof(buffer));
                if (size < 0)
                {
                    fprintf(stderr, "Unable to read from %d\n", _poll[i].fd);
                    continue;
                }

                if (write(_poll[!i].fd, buffer, size) < 0)
                {
                    fprintf(stderr, "Unabel to write to %d\n", _poll[!i].fd);
                    continue;
                }
            }
        }
    }
}