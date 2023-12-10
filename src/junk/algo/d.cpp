#include <iostream>
#include <vector>
#include <algorithm>

#if 0
std::ostream& operator<<(std::ostream& out, const std::vector<int>& v) {
    out << '{';
    for (auto n : v) {
        out << n << ", ";
    }
    out << '}';
    return out;
}
#endif

int main() {
    int n, k, num;
    std::vector<int> sorted;
    std::cin >> n >> k;
    
    sorted.reserve(n);
    while (n-- > 0) {
        std::cin >> num;
        sorted.push_back(num);
    }

    while (k-- > 0) {
        std::cin >> num;
        auto iter = std::lower_bound(sorted.begin(), sorted.end(), num);
        if (iter == sorted.end()) {
            std::cout << (*--iter);
        } else if (iter == sorted.begin() || *iter == num) {
            std::cout << (*iter);
        } else {
            int right = *iter;
            int left = *--iter;
            int left_diff = num - left;
            int right_diff = right - num;
            
            if (left_diff == right_diff || left_diff < right_diff) {
                std::cout << left;
            } else {
                std::cout << right;
            }
        }

        std::cout << '\n';
    }

    return 0;
}