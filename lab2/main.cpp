#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "parent.h"
#include "child.h"

int main() {
    int pipe1[2];  
    int pipe2[2];  


    if (pipe(pipe1) == -1) {
        handle_error("Ошибка создания pipe1");
    }
    if (pipe(pipe2) == -1) {
        handle_error("Ошибка создания pipe2");
    }

    pid_t pid = fork();  
    if (pid == -1) {
        handle_error("Ошибка fork");
    }

    if (pid == 0) {

        child_process(pipe1, pipe2);
    } else {

        std::string filename;
        std::cout << "Введите имя файла для записи: ";
        std::getline(std::cin, filename);
        parent_process(pipe1, pipe2, filename);


        wait(nullptr);
    }

    return 0;
}
