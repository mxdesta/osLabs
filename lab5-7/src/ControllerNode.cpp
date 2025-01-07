#include "../include/ControllerNode.h"
#include "../include/WorkerNode.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

ControllerNode::ControllerNode() : context_(1), socket_(context_, ZMQ_REQ) {
    socket_.connect("tcp://localhost:5555");
}

void ControllerNode::start() {
    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command.empty()) continue;

        if (command.find("create") == 0) {
            int id, parent = -1;
            std::istringstream iss(command);
            std::string cmd;
            iss >> cmd >> id >> parent;

            createNode(id, parent);
        } else if (command.find("exec") == 0) {
            int id;
            std::string params;
            std::istringstream iss(command);
            std::string cmd;
            iss >> cmd >> id;
            std::getline(iss, params);
            execCommand(id, params);
        } else if (command.find("ping") == 0) {
            int id;
            std::istringstream iss(command);
            std::string cmd;
            iss >> cmd >> id;
            pingNode(id);
        } else if (command.find("heartbit") == 0) {
            int time;
            std::istringstream iss(command);
            std::string cmd;
            iss >> cmd >> time;
            heartbit(time);
        } else {
            std::cout << "Error: Unknown command" << std::endl;
        }
    }
}

void ControllerNode::createNode(int id, int parent) {
    if (workerThreads_.find(id) != workerThreads_.end()) {
        std::cout << "Error: Already exists" << std::endl;
        return;
    }

    if (parent != -1 && workerThreads_.find(parent) == workerThreads_.end()) {
        std::cout << "Error: Parent not found" << std::endl;
        return;
    }
    tree_.insert(id, parent);
    launchWorkerNode(id, parent);
    std::cout << "Ok: Node " << id << " created" << std::endl;
}

void ControllerNode::launchWorkerNode(int id, int parentId) {
    workerThreads_[id] = std::thread([id, parentId]() {
        WorkerNode worker(id, parentId);
        worker.run();
    });
    workerThreads_[id].detach(); 
}

void ControllerNode::execCommand(int id, const std::string& params) {
    std::string command = "exec " + std::to_string(id) + " " + params;
    sendCommand(command);
}

void ControllerNode::pingNode(int id) {
    if (workerThreads_.find(id) == workerThreads_.end()) {
        throw std::runtime_error("Node not found");
    }

    std::string command = "ping " + std::to_string(id);
    sendCommand(command);
}

void ControllerNode::heartbit(int time) {
    auto start = std::chrono::steady_clock::now();
    while (true) {
        // Проверяем, прошло ли заданное время
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() >= time) {
            break; // Завершаем heartbit через указанное время
        }

        // Имитируем выполнение heartbit
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (false) {
            std::cout << "Error: Heartbit failed!" << std::endl;  // Сообщение об ошибке
        }
    }
}

void ControllerNode::sendCommand(const std::string& command) {
    // Извлекаем id узла из команды
    std::istringstream iss(command);
    std::string cmd;
    int id;
    iss >> cmd >> id;

    // Подключаемся к узлу
    zmq::socket_t nodeSocket(context_, ZMQ_REQ);
    std::string address = "tcp://localhost:" + std::to_string(5555 + id);
    nodeSocket.connect(address);

    // Отправляем команду
    zmq::message_t request(command.size());
    memcpy(request.data(), command.data(), command.size());
    nodeSocket.send(request, zmq::send_flags::none);

    // Получаем ответ
    zmq::message_t reply;
    auto recv_result = nodeSocket.recv(reply, zmq::recv_flags::none);
    if (!recv_result) {
        std::cerr << "Error: Failed to receive reply from node " << id << std::endl;
        return;
    }

    std::string replyStr(static_cast<char*>(reply.data()), reply.size());
    std::cout << replyStr << std::endl;
}
