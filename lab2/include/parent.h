#ifndef PARENT_H
#define PARENT_H

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>

#define BUFFER_SIZE 1024


void handle_error(const char* msg);


void parent_process(int pipe1[2], int pipe2[2], const std::string& filename);

#endif 
