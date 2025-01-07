cmake_minimum_required(VERSION 3.10)
project(CP)

set(CMAKE_CXX_STANDARD 17)

# Создаем исполняемый файл для родительского процесса
add_executable(CP
    main.cpp
    src/ListAllocator.cpp
    src/BuddyAllocator.cpp
)



# Указываем директорию с заголовочными файлами
target_include_directories(CP PRIVATE include)

