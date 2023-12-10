#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

int main() {
    std::string s;
    int d = ('z' + 1) - 'a';
    int offset = 'a';
    int words_count = 0;
    std::vector<int> uniqe_chars(d, 0);
    std::unordered_set<int> chars;

    while (std::cin >> s) {
        chars.clear();
        words_count++;

        for (auto c : s) {
            const auto& [iter, inserted] = chars.insert(c);
            if (inserted) {
                ++uniqe_chars[c - offset];
            }
        }
    }

    for (int i = 0; i < static_cast<int>(uniqe_chars.size()); ++i) {
        if (uniqe_chars[i] == words_count) {
            std::cout << char(i + offset);
        }
    }
    std::cout << '\n';
    return 0;
}