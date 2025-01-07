cmake_minimum_required(VERSION 3.10)
project(lab5-7)

set(CMAKE_CXX_STANDARD 17)

# Создаем исполняемый файл для родительского процесса
add_executable(lab5-7
    main.cpp
    src/ControllerNode.cpp
    src/WorkerNode.cpp

)



# Указываем директорию с заголовочными файлами
target_include_directories(lab5-7 PRIVATE include)
include_directories(/home/unix/labs/osLabs/cppzmq)  # или путь, куда вы установили cppzmq
target_link_libraries(lab5-7 zmq)