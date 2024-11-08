cmake_minimum_required(VERSION 3.10)
project(Threads)

set(CMAKE_CXX_STANDARD 17)

# Создаем исполняемый файл для родительского процесса
add_executable(Threads
    main.cpp
    src/quicksort.cpp
    src/thread_pool.cpp

)



# Указываем директорию с заголовочными файлами
target_include_directories(Threads PRIVATE include)
