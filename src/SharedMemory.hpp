#ifndef _SHARED_MEMORY_HPP
#define _SHARED_MEMORY_HPP

#include <fcntl.h>
#include <sys/stat.h>

namespace Chat
{
    class SharedMemory
    {
    private:
        int _fd;
        const char *const _name;
        char *_data;
    public:
        static constexpr int DATA_SIZE = 1024;

        explicit SharedMemory(const char *name);
        ~SharedMemory();
        void open(int flag = O_RDWR, mode_t mode = 0660);
        char *getData();
    };
}
#endif
