#include <iostream>
#include <fstream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include <cstring>
#include <string>
#include <sys/wait.h>
#include "../include/common.h"
#include "../include/parent.h"

// Обработчик ошибок
void handleError(const std::string& msg) {
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}

// Обработчик сигналов для родителя
void parentSignalHandler(int sig) {
    if (sig == SIGUSR1) {
        child_ready = 1;
    }
}

bool isValid(const std::string& str) {
    return !str.empty() && std::isupper(str[0]);
}

void processInput(const std::string& filename, const std::string& input) {
    static pid_t child_pid = -1;

    if (child_pid == -1) {
        int fd = shm_open(SHARED_FILE, O_CREAT | O_RDWR, 0666);
        if (fd == -1) handleError("shm_open failed");

        if (ftruncate(fd, BUFFER_SIZE) == -1) handleError("ftruncate failed");

        void* shared_mem = mmap(0, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (shared_mem == MAP_FAILED) handleError("mmap failed");

        close(fd);

        child_pid = fork();
        if (child_pid < 0) {
            handleError("fork failed");
        } else if (child_pid == 0) {
            execl("/home/unix/labs/osLabs/build/lab4/child3", "/home/unix/labs/osLabs/build/lab4/child3", nullptr);
            handleError("execl failed");
        }
        signal(SIGUSR1, parentSignalHandler);
    }

    if (input == "/exit") {
        if (child_pid > 0) {
            int fd = shm_open(SHARED_FILE, O_RDWR, 0666);
            if (fd == -1) handleError("shm_open failed");

            void* shared_mem = mmap(0, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            if (shared_mem == MAP_FAILED) handleError("mmap failed");

            char* shared_data = static_cast<char*>(shared_mem);
            strncpy(shared_data, "/exit", BUFFER_SIZE);
            munmap(shared_mem, BUFFER_SIZE);

            kill(child_pid, SIGUSR1);
            waitpid(child_pid, nullptr, 0); // Ожидаем завершения дочернего процесса
            shm_unlink(SHARED_FILE);       // Удаляем разделяемую память
        }
        exit(0);
    }

    std::string result;
    if (isValid(input)) {
        result = input;
    } else {
        result = "Error: строка должна начинаться с заглавной буквы";
    }

    std::ofstream output_file(filename, std::ios::app);
    if (!output_file.is_open()) {
        handleError("Failed to open file");
    }

    if (result.rfind("Error:", 0) == 0) {
        std::cerr << result << std::endl;
    } else {
        output_file << result << std::endl; // Записываем только корректные строки
    }

    output_file.close();
}
