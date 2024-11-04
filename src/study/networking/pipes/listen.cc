#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define FIFO_PATH "/tmp/my_fifo"
#define PERMISSIONS 0666

int create_and_open_fifo(const char *path, int flags, mode_t mode) {
    if (mkfifo(path, mode) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo");
            return -1;
        }
    }
    int fd = open(path, flags);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    return fd;
}

int main() {
    int fd;
    char buffer[100];

    // Create and open the FIFO for reading
    fd = create_and_open_fifo(FIFO_PATH, O_RDONLY, PERMISSIONS);
    if (fd == -1) {
        return 1;
    }

    // Read from the pipe
    if (read(fd, buffer, sizeof(buffer)) == -1) {
        perror("read");
        close(fd);
        return 1;
    }
    printf("Reader received: %s\n", buffer);

    // Close the pipe
    close(fd);

    // Optionally remove the FIFO
    if (unlink(FIFO_PATH) == -1) {
        perror("unlink");
        return 1;
    }

    return 0;
}
