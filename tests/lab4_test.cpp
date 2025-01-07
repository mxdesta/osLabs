#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstring>
#include "../include/common.h"
#include "../include/parent.h"
#include <cstdio>

// Класс для работы с временными файлами
class TempFile {
public:
    explicit TempFile(const std::string& fName) : filename(fName) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to create temp file: " + filename);
        }
    }

    const std::string& getFilename() const {
        return filename;
    }

    ~TempFile() {
        std::remove(filename.c_str());
    }

private:
    std::string filename;
};

// Чтение содержимого файла
std::string readFileContent(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


void redirectStderrToNull() {
    freopen("/dev/null", "w", stderr);
}

TEST(ParentTest, ProcessInputValidString) {
    TempFile tempFile("test_output.txt");
    std::string input = "HelloWorld2";
    processInput(tempFile.getFilename(), input);

    std::string content = readFileContent(tempFile.getFilename());
    EXPECT_EQ(content, input + "\n");
}

TEST(ParentTest, ProcessInputInvalidString) {
    redirectStderrToNull();
    TempFile tempFile("test_output.txt");
    std::string input = "helloWorld";
    processInput(tempFile.getFilename(), input);

    std::string content = readFileContent(tempFile.getFilename());
    EXPECT_EQ(content, "");
}



int main(int argc, char** argv) {
    // Перенаправляем stdout и stderr в /dev/null
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}