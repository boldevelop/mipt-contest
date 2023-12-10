#include <iostream>
#include <vector>


template <typename Iter>
void Print(Iter b, Iter end) {
    while (b != end) {
        std::cout << (*b++) << ' ';
    }
}

template <typename InIter1, typename InIter2, typename OutIter>
OutIter SetDifference(InIter1 first1, InIter1 last1,
                      InIter2 first2, InIter2 last2,
                      OutIter out) {
    while (first1 != last1) {
        if (first2 == last2 || *first1 < *first2) {
            *out = *first1;
            ++out;
            ++first1;
            continue;
        }
        if (*first1 == *first2) {
            ++first1;
            ++first2;
            continue;
        }
        ++first2;
    }

    return out;
}

int main() {
    std::vector<int> s1 = {1};
    // std::vector<int> s1 = {1, 2, 3, 4, 5};
    // std::vector<int> s2 = {2, 4, 5, 6};
    std::vector<int> s2 = {1, 2};
    std::vector<int> out(s1.size());
    Print(out.begin(), SetDifference(s1.begin(), s1.end(), s2.begin(),
                                     s2.end(), out.begin()));
    std::cout << '\n';
    return 0;
}