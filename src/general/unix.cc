



class unix file {
fd = -1 

// move assignment
UnixFile& operator=(UnixFile&& other) noexcept {
    if (this != &other) {
        if (fd != -1) close(fd);
        fd = other.fd;
        other.fd = -1;
        return *this;
    }
}


~unixfile() {
if (fd != -1) close(fd)
}


}
