#include "Semaphore.hpp"

#include <stdio.h>
#include <stdlib.h>

Chat::Semaphore::Semaphore(const char *name) : _name(name), _value(0), _semaphore(nullptr)
{
}

Chat::Semaphore::~Semaphore()
{
    if (opened())
    {
        sem_close(_semaphore);
        sem_unlink(_name);
    }
}

void Chat::Semaphore::open(int flag, uint value, mode_t mode)
{
    _semaphore = sem_open(_name, flag);
    if (!opened())
    {
        _semaphore = sem_open(_name, O_CREAT | flag, mode, value);
        if (!opened())
        {
            fprintf(stderr, "Unable to open semaphore.\n");
            exit(1);
        }
    }

    update();
}

bool Chat::Semaphore::opened()
{
    return _semaphore;
}

void Chat::Semaphore::down()
{
    if (sem_trywait(_semaphore) < 0)
    {
        fprintf(stderr, "Critical section is occupied now. Wait for it.\n");
        if (sem_wait(_semaphore) < 0)
        {
            fprintf(stderr, "Unable to enter to the critical section.\n");
            exit(1);
        }
    }

    update();
}

void Chat::Semaphore::up()
{
    sem_post(_semaphore);
    update();
}

void Chat::Semaphore::update()
{
    _value = sem_getvalue(_semaphore, &_value);
}

int Chat::Semaphore::getValue()
{
    return _value;
}