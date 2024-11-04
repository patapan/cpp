#include <iostream>
#include <fstream>
#include <fcntl.h> // for open()
#include <unistd.h> // for read(), close()

void read1(const std::string& file) {
    std::ifstream stream(file);

    std::string buffer;
    while (getline(stream, buffer)) {
        std::cout << buffer;
    }
}

void read2(std::string file) {
    int fd = open(file.c_str(), O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    ssize_t bytes_read;

    char buffer[1024];
    while((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) std::cout << buffer[i];
    }

    close(fd);
}

int main() {
    read2("input.txt");
}