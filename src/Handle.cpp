#include "Handle.hpp"

#include <stdlib.h>
#include <signal.h>
#include <string.h>

void Chat::handle(int signal)
{
    exit(1);
}

void Chat::setHandler(int signal, void (*handler)(int))
{
    struct sigaction action;
    bzero(&action, sizeof(action));
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(signal, &action, nullptr);
}