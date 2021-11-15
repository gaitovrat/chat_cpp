#include "Semaphore.hpp"

#include <stdio.h>

int main(int argc, char const *argv[])
{
    Chat::Semaphore sem("/sem1");
    sem.open();

    printf("%s = %d\n", __STRING(sem), sem.getValue());
    sem.up();
    printf("%s = %d\n", __STRING(sem), sem.getValue());
    sem.down();
    printf("%s = %d\n", __STRING(sem), sem.getValue());

    return 0;
}
