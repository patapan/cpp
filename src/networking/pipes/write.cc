#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    const char *fifo_name = "/tmp/my_fifo";
    int fd;
    const char *message = "Hello from writer";

    // Open the named pipe for writing
    fd = open(fifo_name, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Write to the pipe
    write(fd, message, strlen(message) + 1);

    // Close the pipe
    close(fd);

    return 0;
}
