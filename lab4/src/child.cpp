#include "../include/common.h"
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include <cstring>
#include <cctype>
#include <string>

void handleError(const std::string& msg) {
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}

int main() {
    // Подключение к разделяемой памяти
    int fd = shm_open(SHARED_FILE, O_RDWR, 0666);
    if (fd == -1) handleError("shm_open failed");

    void* shared_mem = mmap(0, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_mem == MAP_FAILED) handleError("mmap failed");

    close(fd);

    char* shared_data = static_cast<char*>(shared_mem);
    signal(SIGUSR1, [](int) {});

    while (true) {
        pause();

        if (std::string(shared_data) == "/exit") {
            break;
        }
    }

    munmap(shared_mem, BUFFER_SIZE);
    shm_unlink(SHARED_FILE);
    return 0;
}