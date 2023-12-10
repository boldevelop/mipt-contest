#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

int main() {
    int k;
    std::cin >> k;

    std::string word;
    std::unordered_map<std::string, int> freq;
    while (std::cin >> word) {
        ++freq[word];
    }
    std::vector<std::pair<std::string, int>> data(freq.begin(), freq.end());
    int upper = std::min(k, static_cast<int>(data.size()));
    std::partial_sort(data.begin(), data.begin() + upper,
                      data.end(),
                      [](const auto& a, const auto& b) {
                        return std::tie(b.second, a.first) < std::tie(a.second, b.first);
                      });

    for (int i = 0; i < upper; ++i) {
        const auto& [word, f] = data[i];
        std::cout << word << " " << f << "\n";
    }

    return 0;
}