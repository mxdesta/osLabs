#include "include/parent.h"
#include <iostream>
#include <string>

int main() {
    std::string filename = "output.txt";
    std::string input;

    while (true) {
        std::cout << "Введите строку (/exit для выхода): ";
        std::getline(std::cin, input);

        processInput(filename, input);

        if (input == "/exit") {
            break;
        }
    }

    return 0;
}