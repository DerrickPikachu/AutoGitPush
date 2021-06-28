#include <iostream>
#include <string>
#include <unistd.h>
#include <array>

void execCommand(std::string cmd) {
    auto pipe = popen(cmd.c_str(), "r");
    if (pipe == nullptr) {
        std::cout << "Cannot open pipe" << std::endl;
        return;
    }

    std::array<char, 256> buffer;
    std::string result;

    while (!std::feof(pipe)) {
        auto byte = std::fread(buffer.data(), 1, buffer.size(), pipe);
        result.append(buffer.data(), byte);
    }

    auto record = pclose(pipe);
    int out_exitStatus = 0;

    if (WIFEXITED(record)) {
        out_exitStatus = WEXITSTATUS(record);
    }

    std::cout << result << std::endl;
}

int main(int argc, char* argv[]) {
    std::string gitAdd = "git add .";
    std::string commit = "git commit -m ";
    std::string gitPush = "git push";
    std::string temporary;

    for (int i = 1; i < argc; i++) {
        temporary = temporary + argv[i];
        if (i < argc - 1)
            temporary += " ";
    }

    commit = commit + "\"" + temporary + "\"";

    execCommand(gitAdd);
    std::cout << "Add complete" << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << commit << std::endl;
    execCommand(commit);
    std::cout << "-------------------" << std::endl;

    return 0;
}
