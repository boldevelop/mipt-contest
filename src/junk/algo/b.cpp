#include <iostream>
#include <vector>
#include <algorithm>

template<typename Iter>
Iter adjacentFind(Iter f, Iter l) {
    if (f == l) {
        return l;
    }
    Iter next = f;
    ++next;
    while (next != l) {
        if (*next == *f) {
            return f;
        }
        ++next;
        ++f;
    }

    return l;
}

template<typename Iter>
Iter Unique(Iter first, Iter last) {
    first = adjacentFind(first, last);
    if (first == last) {
        return last;
    }

    Iter same = first;
    ++first;
    while (++first != last) {
        if (*same != *first) {
            *++same = std::move(*first);
        }
    }

    return ++same;
}

template <typename Iter>
void print(Iter b, Iter end) {
    while (b != end) {
        std::cout << (*b++) << ' ';
    }
}

int main() {
    std::vector<int> d = {1, 2, 2, 3, 4, 4, 5, 6, 6};
    print(adjacentFind(d.begin(), d.end()), d.end());
    std::cout << '\n';
    print(d.begin(), Unique(d.begin(), d.end()));
    std::cout << '\n';
}