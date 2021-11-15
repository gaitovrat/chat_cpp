#include <stdio.h>
#include <stdlib.h>

#include "Client.hpp"

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Enter server address and port.\n");
        return 1;
    }

    const char *address = argv[1];
    const int port = atoi(argv[2]);

    if (!address || !port)
    {
        fprintf(stderr, "Unalbe to parse address or port.\n");
        return 1;
    }

    Chat::Client client(port, address);
    client.connect();
    client.loop();
}
