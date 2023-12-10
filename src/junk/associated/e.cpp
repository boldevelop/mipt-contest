#include <iostream>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <unordered_map>

int main() {
    int m, n;
    std::string word;
    std::cin >> m;
    std::cin >> n;
    std::unordered_map<std::string, int> q;
    std::map<int, std::set<std::string>> res;

    while (m > 0) {
        std::cin >> word;
        for (int i = 0; i <= static_cast<int>(word.size() - n); i++) {
            ++q[std::string(word, i, n)];
        }
        m--;
    }

    for (const auto& p : q) {
        auto iter = res.find(p.second);
        if (iter != res.end()) {
            iter->second.insert(p.first);
        } else {
            res[p.second] = {p.first};
        }
    }

    for (auto iter = res.rbegin(); iter != res.rend(); iter = next(iter)) {
        for (const auto& n_gram : iter->second) {
            std::cout << n_gram << " - " << iter->first << '\n';
        }
    }

    return 0;
}