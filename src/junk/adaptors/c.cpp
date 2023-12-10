#include <iostream>
#include <string>
#include <queue>

int main() {
    std::string command;
    std::priority_queue<int> qu;

    while (std::getline(std::cin, command)) {
        auto sp_pos = command.find(' ');
        if (sp_pos == std::string::npos) {
            if (command == "EXTRACT") {
                if (qu.empty()) {
                    std::cout << "CANNOT" << '\n';
                } else {
                    std::cout << qu.top() << '\n';
                    qu.pop();
                }
            } else {
                qu = {};
            }
            continue;
        }

        auto num = std::stoi(std::string(command, sp_pos + 1, command.size() - sp_pos - 1));
        qu.push(num);
    }

    return 0;
}