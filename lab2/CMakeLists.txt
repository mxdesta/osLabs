cmake_minimum_required(VERSION 3.10)
project(ProcessCommunication)

set(CMAKE_CXX_STANDARD 11)

# Создаем исполняемый файл для родительского процесса
add_executable(ProcessCommunication
    main.cpp
    src/parent.cpp
)

# Создаем исполняемый файл для дочернего процесса
add_executable(child src/child.cpp)

# Указываем директорию с заголовочными файлами
target_include_directories(ProcessCommunication PRIVATE include)
target_include_directories(child PRIVATE include)
