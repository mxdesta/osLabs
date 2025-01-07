include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)


set(CMAKE_CXX_STANDARD 17)


#add_executable(lab2_test lab2_test.cpp ../lab2/src/parent.cpp ../lab2/include/parent.h)
#add_executable(lab3_test lab3_test.cpp ../lab3/src/thread_pool.cpp ../lab3/include/thread_pool.h ../lab3/src/quicksort.cpp ../lab3/include/quicksort.h )
#add_executable(lab4_test lab4_test.cpp ../lab4/src/parent.cpp ../lab4/include/parent.h ../lab4/include/common.h ../lab4/src/common.cpp)
#add_executable(lab5_test lab5_test.cpp)

add_executable(lab5-7_test lab5-7_test.cpp ../lab5-7/src/WorkerNode.cpp ../lab5-7/src/ControllerNode.cpp)
#add_executable(CP_test CP_tests.cpp ../CP/src/BuddyAllocator.cpp ../CP/include/Allocator.h  ../CP/src/ListAllocator.cpp)


#target_link_libraries(lab2_test PRIVATE GTest::GTest)
#target_include_directories(lab2_test PRIVATE ../lab2/include)

#target_link_libraries(lab3_test PRIVATE GTest::GTest)
#target_include_directories(lab3_test PRIVATE ../lab3/include)

#target_link_libraries(lab4_test PRIVATE GTest::GTest)
#target_include_directories(lab4_test PRIVATE ../lab4/include)

#target_link_libraries(lab5_test PRIVATE GTest::GTest)
#target_include_directories(lab5_test PRIVATE ../lab5/include)
#target_link_libraries(lab5_test PRIVATE lib1 m dl)

target_link_libraries(lab5-7_test PRIVATE GTest::GTest zmq)
target_include_directories(lab5-7_test PRIVATE ../lab5-7/include)
include_directories(/home/unix/labs/osLabs/cppzmq)  # или путь, куда вы установили cppzmq


#target_link_libraries(CP_test PRIVATE GTest::GTest)
#target_include_directories(CP_test PRIVATE ../CP/include)


#add_test(lab3_test lab3_test)
#add_test(lab2_test lab2_test)
#add_test(lab4_test lab4_test)
#add_test(lab5_test lab5_test)
add_test(lab5-7_test lab5-7_test)
#add_test(CP_test CP_test)


