#include <iostream>
#include <set>
#include <map>
#include <string>

int main() {
    std::map<int, std::set<std::string>> dict;
    int n, num;
    std::string word;
    std::cin >> n;
    while (n > 0) {
        std::cin >> word >> num;
        auto iter = dict.find(num);
        if (iter != dict.end()) {
            iter->second.insert(word);
        } else {
            dict[num] = std::set({word});
        }
        n--;
    }

    for (const auto& iter : dict) {
        std::cout << iter.first << " ";
        for (const auto& w : iter.second) {
            std::cout << w << " ";
        }
        std::cout << '\n';
    }
    return 0;
}