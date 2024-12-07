#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <cstring>
#include <fstream>

void ParentMain() {
    int pipe1[2], pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        close(pipe1[1]);
        close(pipe2[0]);
        execl("/home/denis/Рабочий стол/os/OS-labs-template/build/lab2/child", "child", std::to_string(pipe1[0]).c_str(), std::to_string(pipe2[1]).c_str(), nullptr);
        perror("Execl failed");
        exit(1);
    } else { 
        close(pipe1[0]);
        close(pipe2[1]);

        std::string filename;
        std::cout << "Enter filename: ";
        std::getline(std::cin, filename);

        std::ofstream outfile(filename);
        if (!outfile) {
            std::cerr << "Cannot open file for writing.\n";
            return;
        }

        std::string line;
        std::cout << "Enter text lines (type 'exit' to stop):" << std::endl;
        while (std::getline(std::cin, line)) {
            if (line == "exit") break;

            write(pipe1[1], line.c_str(), line.size() + 1);
            char buffer[256];
            ssize_t bytesRead = read(pipe2[0], buffer, sizeof(buffer) - 1); 
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                if (strncmp(buffer, "Error:", 6) == 0) {
                    std::cout << "Child error: " << buffer << std::endl;
                } else {
                    outfile << buffer; 
                }
            }
        }
        
        close(pipe1[1]);
        close(pipe2[0]);
        wait(nullptr); 
    }
}
