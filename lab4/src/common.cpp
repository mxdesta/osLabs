#include "../include/common.h"

const char* SHARED_FILE = "/shared_memory";
const size_t BUFFER_SIZE = 1024;
volatile sig_atomic_t child_ready = 0;