#include <gtest/gtest.h>
#include <array>
#include <fstream>
#include <memory>
#include "parent.h"

class TempFile {
public:
    TempFile(const std::string& fName) : filename(fName) {
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

std::string readFileContent(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

TEST(ProcessTest, ValidInputTest) {
    int pipefd[2];
    ASSERT_EQ(pipe(pipefd), 0) << "Pipe creation failed";
    TempFile tempFile("output.txt");

    std::istringstream input_stream("output.txt\nHello World\nexit\n");
    std::cin.rdbuf(input_stream.rdbuf());
    ParentMain();

    std::string content = readFileContent(tempFile.getFilename());
    EXPECT_EQ(content, "Hello World");
}

TEST(ProcessTest, InvalidInputTest) {
    int pipefd[2];
    ASSERT_EQ(pipe(pipefd), 0) << "Pipe creation failed";
    TempFile tempFile("output.txt");

    std::istringstream input_stream("output.txt\nhello world\nexit\n");
    std::cin.rdbuf(input_stream.rdbuf());
    ParentMain();

    std::string content = readFileContent(tempFile.getFilename());
    EXPECT_EQ(content, "");  // Invalid line should not be written to file
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
