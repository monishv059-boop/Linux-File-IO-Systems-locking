#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    struct flock lock;

    fd = open("locked.txt", O_RDWR | O_CREAT, 0644);
    if (fd < 0)
    {
        printf("Error opening file!\n");
        return 1;
    }

    // Set write lock
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    printf("Locking file...\n");

    if (fcntl(fd, F_SETLK, &lock) == -1)
    {
        printf("File already locked by another process!\n");
        close(fd);
        return 1;
    }

    printf("File locked successfully!\n");

    // Write to file
    write(fd, "This is locked content", 23);
    printf("Data written to file\n");

    printf("Press Enter to unlock...");
    getchar();

    // Unlock file
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    printf("File unlocked!\n");

    close(fd);
    return 0;
}
