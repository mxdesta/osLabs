#include <iostream>
#include "include/functions.h"

int main() {
    int command;
    std::cout << "Введите команду: ";
    std::cin >> command;

    if (command == 1) {
        int K;
        std::cout << "Введите K: ";
        std::cin >> K;
        std::cout << "Pi = " << Pi(K) << std::endl;
    } else if (command == 2) {
        float A, B;
        std::cout << "Введите A и B: ";
        std::cin >> A >> B;
        std::cout << "Площадь = " << Square(A, B) << std::endl;
    }

    return 0;
}
