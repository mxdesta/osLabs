#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cctype>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: child <readPipe> <writePipe>\n";
        return 1;
    }

    int readPipe = std::stoi(argv[1]);
    int writePipe = std::stoi(argv[2]);

    char buffer[256];
    ssize_t bytesRead;


    while ((bytesRead = read(readPipe, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';


        if (isupper(buffer[0])) {
            write(writePipe, buffer, bytesRead);
        } else {
            const char *errorMsg = "Error: Line must start with an uppercase letter.\n";
            write(writePipe, errorMsg, strlen(errorMsg));
        }
    }

    close(readPipe);
    close(writePipe);
    return 0;
}
