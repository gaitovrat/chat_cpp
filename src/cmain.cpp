#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

constexpr const char *MEMORY_NAME = "/shared";
constexpr int SIZE = 1024;

int main(int argc, char const *argv[])
{
    const char *message = "Hello world!";

    printf("Create shared memory.\n");
    int fd = shm_open(MEMORY_NAME, O_RDONLY, 0660);
    if (fd < 0)
    {
        fprintf(stderr, "Unable to open shared memory.\n");
    }

    printf("Get shared memory.\n");
    char *data = (char *)mmap(nullptr, SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (!data)
    {
        fprintf(stderr, "Unable to allocate shared memory.\n");
        return 1;
    }

    printf("%s\n", data);

    munmap(data, SIZE);
    close(fd);

    return 0;
}
