#include <gtest/gtest.h>
#include "../include/ControllerNode.h"
#include "../include/WorkerNode.h"
#include <zmq.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <atomic>
#include <future>

// Тест для проверки создания узла
TEST(NodeTest, CreateNode) {
    ControllerNode controller;
    controller.createNode(10);
    controller.createNode(20, 10);

    EXPECT_NO_THROW(controller.pingNode(10));
    EXPECT_NO_THROW(controller.pingNode(20));
}

// Тест для проверки выполнения команды
TEST(NodeTest, ExecCommand) {
    ControllerNode controller;
    controller.createNode(10);
    controller.execCommand(10, "3 1 2 3");

    EXPECT_NO_THROW(controller.execCommand(10, "3 1 2 3"));
}

// Тест для проверки доступности узла
TEST(NodeTest, PingNode) {
    ControllerNode controller;
    controller.createNode(10);

    EXPECT_NO_THROW(controller.pingNode(10));
}

// Тест для проверки недоступности узла
TEST(NodeTest, PingNonExistentNode) {
    ControllerNode controller;

    EXPECT_THROW(controller.pingNode(100), std::runtime_error);
}

// Тест для проверки heartbit
TEST(NodeTest, Heartbit) {
    ControllerNode controller;

    // Перенаправляем стандартный вывод в строковый поток
    std::stringstream buffer;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());


    auto future = std::async(std::launch::async, [&controller]() {
        controller.heartbit(5000); 
    });

    // Ожидаем 5 секунд
    auto status = future.wait_for(std::chrono::seconds(5));

    std::string output = buffer.str();
    bool errorMessageDetected = !output.empty();
    std::cout.rdbuf(oldCoutStreamBuf);

    // Если heartbit все еще работает (не завершился за 5 секунд) и вывод пустой, тест успешен
    if (status == std::future_status::timeout && !errorMessageDetected) {
        SUCCEED();
    } else if (status == std::future_status::ready) {
        if (errorMessageDetected) {
            FAIL() << "Heartbit finished early and printed an error message: " << output;
        } else {
            FAIL() << "Heartbit finished early but no error messages were printed.";
        }
    } else {
        FAIL() << "Unexpected state: Heartbit is still running, but an error message was printed: " << output;
    }

    if (status == std::future_status::timeout) {
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}