#include <unordered_set>
#include <vector>
#include <iostream>

int main() {
    std::unordered_set<int> nums;
    std::vector<int> inputs;
    int num;

    while (std::cin >> num) {
        inputs.push_back(num);
    }

    for (auto n : inputs) {
        const auto& [iter, inserted] = nums.insert(n);
        if (inserted) {
            std::cout << "NO\n";
        } else {
            std::cout << "YES\n";
        }
    }
    
    return 0;
}

