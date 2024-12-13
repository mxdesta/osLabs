#include <gtest/gtest.h>
#include <dlfcn.h>
#include <cmath>
#include "../lab5/include/functions.h"

TEST(PiTest, Implementation1) {
    
    int K = 100000;
    float result = Pi(K);
    float expected = M_PI;
    EXPECT_NEAR(result, expected, 1e-5);
}

TEST(PiTest, Implementation2) {
    const char* pathToLib2 = "/home/unix/labs/osLabs/build/lab5/lib2.so";
    ASSERT_NE(pathToLib2, nullptr) << "Переменная окружения PATH_TO_LIB2 не установлена";

    void* handle = dlopen(pathToLib2, RTLD_LAZY);
    ASSERT_NE(handle, nullptr) << "Ошибка загрузки lib2: " << dlerror();

    using PiFunc = float(*)(int);
    PiFunc PiLib2 = reinterpret_cast<PiFunc>(dlsym(handle, "Pi"));
    ASSERT_NE(PiLib2, nullptr) << "Ошибка загрузки функции Pi из lib2: " << dlerror();

    int K = 1000000; 
    float result = PiLib2(K);
    float expected = M_PI;
    EXPECT_NEAR(result, expected, 1e-5);  // Сохраняем погрешность 1e-5

    dlclose(handle);
}

TEST(SquareTest, Implementation1) {
    // Тестируем Square из lib1 (прямоугольник)
    float A = 10.0f;
    float B = 5.0f;
    float result = Square(A, B);
    float expected = A * B;  // Площадь прямоугольника
    EXPECT_NEAR(result, expected, 1e-5);
}

TEST(SquareTest, Implementation2) {
    // Тестируем Square из lib2 (прямоугольный треугольник)
    const char* pathToLib2 = "/home/unix/labs/osLabs/build/lab5/lib2.so";
    ASSERT_NE(pathToLib2, nullptr) << "Переменная окружения PATH_TO_LIB2 не установлена";

    void* handle = dlopen(pathToLib2, RTLD_LAZY);
    ASSERT_NE(handle, nullptr) << "Ошибка загрузки lib2: " << dlerror();

    using SquareFunc = float(*)(float, float);
    SquareFunc SquareLib2 = reinterpret_cast<SquareFunc>(dlsym(handle, "Square"));
    ASSERT_NE(SquareLib2, nullptr) << "Ошибка загрузки функции Square из lib2: " << dlerror();

    float A = 10.0f;
    float B = 5.0f;
    float result = SquareLib2(A, B);
    float expected = 0.5f * A * B; 
    EXPECT_NEAR(result, expected, 1e-5);

    dlclose(handle);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
