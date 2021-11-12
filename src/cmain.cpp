#include "Client.hpp"

#include <unistd.h>
#include <stdio.h>
#include <string.h>

enum Pipe
{
    READ = 0,
    WRITE,
    SUCCESS = 0
};

int main(int argc, char const *argv[])
{
    int _pipe[2];
    char buffer[10];
    char *str = "1,2,3,4,5~6,7,8,9,10";

    if (pipe(_pipe) != Pipe::SUCCESS)
    {
        printf("Error in pipe()\n");
        return 1;
    }

    if (write(_pipe[Pipe::WRITE], str, strlen(str)) < 0) 
    {
        printf("Error in write()\n");
        return 1;
    }

#define READ read(_pipe[Pipe::READ], buffer, sizeof(buffer) - 1)
    for (int size = READ; size > 0; size = READ)
    {
        buffer[size] = 0;
        printf("%s\n", buffer);
    }

    return 0;
}
