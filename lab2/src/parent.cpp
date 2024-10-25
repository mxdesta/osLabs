#include "parent.h"

void handle_error(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void parent_process(int pipe1[2], int pipe2[2], const std::string& filename) {
    close(pipe1[0]);  
    close(pipe2[1]);  

   
    int file_fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd == -1) {
        handle_error("Ошибка открытия файла");
    }

  
    dup2(file_fd, STDOUT_FILENO);

  
    std::string input;
    std::cin.ignore();  
    while (true) {
        std::cout << "Введите строку (или 'exit' для выхода): ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        write(pipe1[1], input.c_str(), input.length() + 1);  
    }

    close(pipe1[1]);  
  
    wait(nullptr);

  
    char error_buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(pipe2[0], error_buffer, BUFFER_SIZE)) > 0) {
        error_buffer[bytes_read] = '\0';
        std::cerr << "Родительский процесс получил ошибку: " << error_buffer << std::endl;
    }

    close(pipe2[0]);  
    close(file_fd);   
}
