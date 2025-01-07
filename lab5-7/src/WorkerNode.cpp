#include "../include/WorkerNode.h"
#include <iostream>

WorkerNode::WorkerNode(int id, int parentId) : id_(id), parentId_(parentId), context_(1), socket_(context_, ZMQ_REP), running_(true) {
    // Узел слушает сообщения на уникальном адресе
    std::string address = "tcp://*:" + std::to_string(5555 + id);
    socket_.bind(address);
    std::cout << "Node " << id_ << " started and listening on " << address << std::endl;
}

void WorkerNode::run() {
    std::cout << "Node " << id_ << " started" << std::endl;
    while (true) {
        zmq::message_t request;
        auto recv_result = socket_.recv(request, zmq::recv_flags::none);
        if (!recv_result) {
            std::cerr << "Node " << id_ << ": Failed to receive message" << std::endl;
            continue;
        }

        std::string command(static_cast<char*>(request.data()), request.size());
        std::string response = processCommand(command);

        zmq::message_t reply(response.size());
        memcpy(reply.data(), response.data(), response.size());
        socket_.send(reply, zmq::send_flags::none);
    }
}

std::string WorkerNode::processCommand(const std::string& command) {
    if (command.find("exec") == 0) {
        std::istringstream iss(command);
        std::string cmd;
        int requestedId, n;
        iss >> cmd >> requestedId >> n;

        if (requestedId != id_) {
            return "Error:" + std::to_string(requestedId) + ": Node not found";
        }

        int sum = 0;
        for (int i = 0; i < n; ++i) {
            int num;
            iss >> num;
            sum += num;
        }

        return "Ok:" + std::to_string(id_) + ": " + std::to_string(sum);
    } else if (command.find("ping") == 0) {
        int requestedId;
        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd >> requestedId;

        if (requestedId != id_) {
            return "Error:" + std::to_string(requestedId) + ": Node not found";
        }

        return "Ok:1";
    } else if (command.find("heartbit") == 0) {
        return "Ok";
    }
    return "Error: Unknown command";
}