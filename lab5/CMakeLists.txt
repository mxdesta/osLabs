set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Компиляция первой библиотеки (lib1)
add_library(lib1 SHARED src/lib1.cpp)
set_target_properties(lib1 PROPERTIES OUTPUT_NAME "lib1" PREFIX "")
target_include_directories(lib1 PRIVATE include)

# Компиляция второй библиотеки (lib2)
add_library(lib2 SHARED src/lib2.cpp)
set_target_properties(lib2 PROPERTIES OUTPUT_NAME "lib2" PREFIX "")
target_include_directories(lib2 PRIVATE include)

# Компиляция первой программы (program1)
add_executable(prog1 program1.cpp)
target_include_directories(prog1 PRIVATE include)
target_link_libraries(prog1 PRIVATE lib1 m)  # Линковка с первой библиотекой

# Компиляция второй программы (program2)
add_executable(prog2 program2.cpp)
target_include_directories(prog2 PRIVATE include)
target_link_libraries(prog2 PRIVATE dl m)  # Линковка с динамическим линковщиком (dl) и математической библиотекой (m)