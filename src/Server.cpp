#include "Server.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

Chat::Server::Server(int port) :
        _socket(0), _mutex("/mutex"), _full("/full"), _empty("/empty"), _shm(
                "/chat") {
    _mutex.open(O_RDWR, 1);
    _empty.open(O_RDWR, 1);
    _full.open();
    _shm.open();

    _poll[0].fd = STDIN_FILENO;
    _poll[1].fd = -1;
    for (int i = 0; i < 2; ++i)
        _poll[i].events = POLLIN;

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1) {
        fprintf(stderr, "Unable to open socket.\n");
        exit(1);
    }

    int options = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(options))
            < 0) {
        fprintf(stderr, "Unable to set options.\n");
        exit(1);
    }

    _address.sin_family = AF_INET;
    _address.sin_port = htons(port);
    _address.sin_addr = { INADDR_ANY };
}

Chat::Server::~Server() {
    close(_socket);
}

void Chat::Server::bind() {
    if (::bind(_socket, (const sockaddr*) &_address, sizeof(_address)) < 0) {
        fprintf(stderr, "Unable to bind socket.\n");
        exit(1);
    }
}

void Chat::Server::listen() {
    if (::listen(_socket, 1) < 0) {
        fprintf(stderr, "Unable to listen socket.\n");
        exit(1);
    }
}

void Chat::Server::loop() {
    while (true) {
        int client = -1;
        _poll[1].fd = _socket;

        if (_full.tryDown() == 0) {
            _mutex.down();
            printf("%s", _shm.getData());
            for (int i = 0; i < _processes.size(); ++i) {
                write(_processes[i]->socket, _shm.getData(),
                        Chat::SharedMemory::DATA_SIZE);
            }
            memset(_shm.getData(), 0, Chat::SharedMemory::DATA_SIZE);
            _mutex.up();
            _empty.up();
        }

        if (poll(&_poll[1], 1, 10) < 0) {
            fprintf(stderr, "Unable to poll.\n");
            return;
        }

        if (_poll[1].revents & POLLIN) {
            sockaddr_in client_address;
            int size = sizeof(client_address);

            client = accept(_poll[1].fd, (sockaddr*) &client_address,
                    (socklen_t*) &size);
            if (client == -1) {
                fprintf(stderr, "Unable to connect client.\n");
                return;
            }

            Chat::Process *process = new Chat::Process;
            process->socket = client;
            _processes.push_back(process);

            process->pid = fork();

            if (process->pid == 0) {
                _poll[1].fd = client;
                communication();
            }
        }

        for (size_t i = 0; i < _processes.size(); ++i) {
            int response = waitpid(_processes[i]->pid, nullptr, WNOHANG);
            if (response > 0) {
                delete _processes[i];
                _processes.erase(_processes.begin() + i);
            }
        }
    }
}

void Chat::Server::communication() {
    while (true) {
        if (poll(&_poll[1], 1, -1) < 0) {
            fprintf(stderr, "Unable to poll 2.\n");
            return;
        }

        char buffer[Chat::Server::SIZE];
        if (_poll[1].revents & POLLIN) {
            _empty.down();
            _mutex.down();

            int size = read(_poll[1].fd, buffer, sizeof(buffer));
            if (size <= 0) {
                fprintf(stderr, "Unable to read from %d\n", _poll[1].fd);
                _mutex.up();
                _full.up();
                exit(1);
            }

            if (snprintf(_shm.getData(), Chat::SharedMemory::DATA_SIZE,
                    "[%d] %s", _poll[1].fd, buffer) <= 0) {
                fprintf(stderr, "Unable to write to shared memory\n");
                _mutex.up();
                _full.up();
                exit(1);
            }

            _mutex.up();
            _full.up();
        }

    }
}
