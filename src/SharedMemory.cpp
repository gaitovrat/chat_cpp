#include "SharedMemory.hpp"

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

Chat::SharedMemory::SharedMemory(const char *name) : _fd(-1), _name(name), _data(nullptr)
{}

Chat::SharedMemory::~SharedMemory()
{
    if (_fd >= 0)
    {
        close(_fd);
    }

    if (_data)
    {
        munmap(_data, Chat::SharedMemory::DATA_SIZE);
    }
}

void Chat::SharedMemory::open(int flag, mode_t mode)
{
    _fd = shm_open(_name, flag, mode);
    if (_fd < 0)
    {
        _fd = shm_open(_name, O_CREAT | flag, mode);
        if (_fd < 0)
        {
            fprintf(stderr, "Unable to open shared memory.\n");
            exit(1);
        }

        ftruncate(_fd, Chat::SharedMemory::DATA_SIZE);
    }
}

char *Chat::SharedMemory::getData()
{
    return _data;
}