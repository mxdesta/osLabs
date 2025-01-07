#include <iostream>
#include <dlfcn.h>
#include <string>
#include <cstdlib>

using PiFunc = float(*)(int);
using SquareFunc = float(*)(float, float);

void* loadLibrary(const std::string& path, PiFunc& piFunc, SquareFunc& squareFunc) {
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Ошибка загрузки библиотеки: " << dlerror() << std::endl;
        exit(EXIT_FAILURE);
    }

    piFunc = reinterpret_cast<PiFunc>(dlsym(handle, "Pi"));
    if (!piFunc) {
        std::cerr << "Ошибка загрузки функции Pi: " << dlerror() << std::endl;
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

    squareFunc = reinterpret_cast<SquareFunc>(dlsym(handle, "Square"));
    if (!squareFunc) {
        std::cerr << "Ошибка загрузки функции Square: " << dlerror() << std::endl;
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

    return handle;
}

int main() {
    std::string lib1Path = "./lib1.so";
    std::string lib2Path = "./lib2.so";

    void* currentLib = nullptr;
    PiFunc piFunc = nullptr;
    SquareFunc squareFunc = nullptr;

    // Загружаем первую библиотеку по умолчанию
    currentLib = loadLibrary(lib1Path, piFunc, squareFunc);
    bool usingLib1 = true;

    std::cout << "Введите команды: \n";
    std::cout << "0 - переключить библиотеку\n";
    std::cout << "1 K - вычислить Pi с длиной ряда K\n";
    std::cout << "2 A B - вычислить площадь фигуры\n";

    std::string command;
    while (true) {
        std::cout << "\nВведите команду: ";
        if (!std::getline(std::cin, command) || command.empty()) {
            continue;
        }

        if (command[0] == '0') {
            // Переключение библиотеки
            dlclose(currentLib);
            if (usingLib1) {
                currentLib = loadLibrary(lib2Path, piFunc, squareFunc);
                usingLib1 = false;
                std::cout << "Переключено на lib2.so\n";
            } else {
                currentLib = loadLibrary(lib1Path, piFunc, squareFunc);
                usingLib1 = true;
                std::cout << "Переключено на lib1.so\n";
            }
        } else if (command[0] == '1') {
            // Вызов функции Pi
            try {
                int K = std::stoi(command.substr(2));
                float result = piFunc(K);
                std::cout << "Pi(" << K << ") = " << result << std::endl;
            } catch (...) {
                std::cerr << "Ошибка: Неверный формат команды. Ожидается '1 K'.\n";
            }
        } else if (command[0] == '2') {
            // Вызов функции Square
            try {
                size_t pos = command.find(' ', 2);
                float A = std::stof(command.substr(2, pos - 2));
                float B = std::stof(command.substr(pos + 1));
                float result = squareFunc(A, B);
                std::cout << "Square(" << A << ", " << B << ") = " << result << std::endl;
            } catch (...) {
                std::cerr << "Ошибка: Неверный формат команды. Ожидается '2 A B'.\n";
            }
        } else {
            std::cout << "Неизвестная команда. Попробуйте снова.\n";
        }
    }

    if (currentLib) {
        dlclose(currentLib);
    }

    return 0;
}
