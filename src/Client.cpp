#include "Client.hpp"

#include <stdio.h>

Chat::Client::Client()
{
    printf("I am a Client\n");
}

Chat::Client::~Client()
{
    printf("Client is die\n");
}