#ifndef _SEMAPHORE_HPP
#define _SEMAPHORE_HPP

#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace Chat 
{
    class Semaphore 
    {
    private:
        const char *const _name;
        int _value;
        sem_t *_semaphore;
    public:
        Semaphore(const char *name);
        ~Semaphore();
        void open(int flag = O_RDWR, uint value = 0, mode_t mode = 0660);
        bool opened();
        void down();
        void up();
        int getValue();

    private:
        void update();
    };
}
#endif