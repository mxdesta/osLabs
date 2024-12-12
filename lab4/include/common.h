#ifndef COMMON_H
#define COMMON_H

#include <cstddef>
#include <csignal>

extern const char* SHARED_FILE;
extern const size_t BUFFER_SIZE; 
extern volatile sig_atomic_t child_ready;

#endif // COMMON_H