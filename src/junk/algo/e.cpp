#include <iostream>
#include <vector>
#include <algorithm>


template <typename Iter>
void print(Iter b, Iter end) {
    while (b != end) {
        std::cout << (*b++) << ' ';
    }
}

template <typename Container>
void Print(const Container& data, std::string dmtr) {
    auto end = data.end();
    for (auto b = data.begin(); b != end; b = std::next(b)) {
        if (b != data.begin()) {
            std::cout << dmtr;
        }
        std::cout << *b;
    }
    std::cout << '\n';
}

int main() {
    std::vector<int> data = {-2, -1, 0, 1, 3, 4, 5};
    std::vector<int> filtered(data.size());

    auto filteredLast = std::copy_if(
        data.begin(),
        data.end(),
        filtered.begin(),
        [](const int& x) { return x > 0; }
    );

    Print(filtered, ", ");
    // std::cout << '\n';
    
    return 0;
}
