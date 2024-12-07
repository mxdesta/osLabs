#ifndef CHILD_H
#define CHILD_H

#include <string>
#include <unistd.h>
#include <iostream>
#include <cctype>  

#define BUFFER_SIZE 1024


void child_process(int pipe1[2], int pipe2[2]);


bool is_valid_string(const std::string& str);

#endif 
