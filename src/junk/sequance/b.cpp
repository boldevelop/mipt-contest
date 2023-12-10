#include <iostream>
#include <string>
#include <deque>

int main() {
    int n, k;
    std::string name, dir;
    std::cin >> n;
    std::deque<std::string> names;

    while (n-- > 0) {
        std::cin >> name >> dir;
        if (dir == "top") {
            names.push_front(std::move(name));
        } else {
            names.push_back(std::move(name));
        }
    }

    std::cin >> k;
    int num;
    while (k-- > 0) {
        std::cin >> num;
        std::cout << names[num - 1] << '\n'; 
    }
    return 0;
}