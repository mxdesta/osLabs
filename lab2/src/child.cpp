#include "child.h"

bool is_valid_string(const std::string& str) {
    if (str.empty()) {
        return false;  
    }
    return isupper(str[0]);  
}

void child_process(int pipe1[2], int pipe2[2]) {
    close(pipe1[1]);  
    close(pipe2[0]);  

    
    dup2(pipe2[1], STDERR_FILENO);

 
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(pipe1[0], buffer, BUFFER_SIZE)) > 0) {
        buffer[bytes_read] = '\0';

        if (is_valid_string(buffer)) {
            std::cout << "Дочерний процесс: " << buffer << std::endl;
        } else {
            std::cerr << "Ошибка: строка не начинается с заглавной буквы" << std::endl;
        }
    }

    close(pipe1[0]);  
    close(pipe2[1]);  
}
