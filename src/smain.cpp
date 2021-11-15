#include "SharedMemory.hpp"

#include <stdio.h>

int main(int argc, char const *argv[])
{
    Chat::SharedMemory shm("/shm1");
    shm.open();

    snprintf(shm.getData(), Chat::SharedMemory::DATA_SIZE, "Hello world!\n");

    return 0;
}
