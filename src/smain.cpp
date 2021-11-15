#include <stdio.h>
#include <stdlib.h>

#include "Server.hpp"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Enter port number.\n");
        return 1;
    }

    int port = atoi(argv[1]);
    if (!port)
    {
        fprintf(stderr, "Unable to parse port.\n");
        return 1;
    }

    Chat::Server server(port);
    server.bind();
    server.listen();
    server.loop();

    return 0;
}
