#include <string>
#include <string_view>
#include <iostream>

bool NextToken(std::string_view& sv, const char delimiter,
               std::string_view& token) {
    if (sv.empty()) {
        return false;
    }

    auto pos = sv.find(delimiter);
    token = sv.substr(0, pos);
    sv = pos == std::string_view::npos ? std::string_view{} : sv.substr(pos + 1);

    return true;
}

int main() {
    std::string_view sv = "Hello world and good bye";

    const char delimiter = ' ';
    std::string_view token;

    while (NextToken(sv, delimiter, token)) {
        std::cout << token << "\n";
    }
}