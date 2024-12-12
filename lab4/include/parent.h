#ifndef PARENT_H
#define PARENT_H

#include <string>

void processInput(const std::string& filename, const std::string& input);
void parentSignalHandler(int sig);
void handleError(const std::string& msg);

#endif
