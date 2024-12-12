cmake_minimum_required(VERSION 3.10)
project(PC3)

set(CMAKE_CXX_STANDARD 17)

# Создаем исполняемый файл для родительского процесса
add_executable(PC3
    main.cpp
    src/parent.cpp
    src/common.cpp
    include/common.h
    include/parent.h
)

# Создаем исполняемый файл для дочернего процесса
add_executable(child3 src/child.cpp src/common.cpp include/common.h)

# Указываем директорию с заголовочными файлами
target_include_directories(PC3 PRIVATE include)
target_include_directories(child3 PRIVATE include)
