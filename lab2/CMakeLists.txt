cmake_minimum_required(VERSION 3.10)  # Минимальная версия CMake
project(ProcessCommunication)          # Название проекта

set(CMAKE_CXX_STANDARD 11)             # Установка стандарта C++

# Создаем исполняемый файл из всех исходных файлов
add_executable(ProcessCommunication
    main.cpp
    src/parent.cpp
    src/child.cpp
)

# Указываем директорию с заголовочными файлами
target_include_directories(ProcessCommunication PRIVATE include)
