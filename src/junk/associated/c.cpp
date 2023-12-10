#include <iostream>
#include <string>
#include <string_view>
#include <set>
#include <vector>

std::vector<int> get_sep_posns(const std::string& p, char sep = '/') {
    std::vector<int> res;
    std::string::size_type pos;
    int c_pos = 0;
    while ((pos = p.find(sep, c_pos)) != std::string::npos) {
        res.push_back(pos);
        c_pos = pos + 1;
    }
    return res;
}

#ifndef P_V
std::ostream& operator<< (std::ostream& out, const std::vector<int>& v) {
    out << "[";
    for (auto& c : v) {
        out << c << ", ";
    }
    out << "]";

    return out;
}
#endif

int main() {
    std::string path;
    std::set<std::string> dir_path;

    while (std::cin >> path) {
        auto res = get_sep_posns(path);
        dir_path.insert("/");

        for (int i = 1; i < static_cast<int>(res.size()); ++i) {
            dir_path.insert(std::string(path, 0, res[i] + 1));
        } 
    }

    for (const auto& d_p : dir_path) {
        std::cout << d_p << '\n';
    }

    return 0;
}