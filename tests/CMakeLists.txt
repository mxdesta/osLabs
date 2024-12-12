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
add_executable(lab4_test lab4_test.cpp ../lab4/src/parent.cpp ../lab4/include/parent.h ../lab4/include/common.h ../lab4/src/common.cpp)


#target_link_libraries(lab2_test PRIVATE GTest::GTest)
#target_include_directories(lab2_test PRIVATE ../lab2/include)

#target_link_libraries(lab3_test PRIVATE GTest::GTest)
#target_include_directories(lab3_test PRIVATE ../lab3/include)

target_link_libraries(lab4_test PRIVATE GTest::GTest)
target_include_directories(lab4_test PRIVATE ../lab4/include)

#add_test(lab3_test lab3_test)
#add_test(lab2_test lab2_test)
add_test(lab4_test lab4_test)