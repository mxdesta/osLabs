#ifndef WORKERNODE_H
#define WORKERNODE_H

#include <zmq.hpp>
#include <string>
#include <atomic>

class WorkerNode {
public:
    WorkerNode(int id, int parentId = -1);
    void run();

private:
    std::string processCommand(const std::string& command);

    int id_;
    int parentId_;
    zmq::context_t context_;
    zmq::socket_t socket_;
    std::atomic<bool> running_;
};

#endif