#include <iostream>
#include <string>
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


/**
 * How to use this program?
 * You must first type "git init" under your project root,
 * then try "git status" to check the repository init successfully.
 * After that, use "git remote add origin 'repo'" to specify the repository on the github,
 * and try to push to the repo one time. Then you can start your development.
 * When you want to push the new content again, you can only execute the exe of this program.
 * For example:
 * ./autopush my commit
 * Then the commit content will be the string following the './autopush'
 * @param argc
 * @param argv
 * @return
 */

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
    execCommand(gitPush);

    return 0;
}
