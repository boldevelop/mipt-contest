#include <iostream>
#include <list>
#include <iterator>
#include <string>

class Editor {
public:
    Editor() {
        std::string line;
        while (std::getline(std::cin, line)) {
            auto s = line.size();
            text_.push_back(std::move(line));
            if (s == 0) {
                break;
            }
        }
        cursor_ = text_.begin();
        buffer_ = "";
    }

    void ReadCommands() {
        std::string line;
        while (std::getline(std::cin, line)) {
            if (line == "Down") {
                ApplyDown();
            } else if (line == "Up") {
                ApplyUp();
            } else if (line == "Ctrl+V") {
                ApplyInsert();
            } else if (line == "Ctrl+X") {
                ApplyCut();
            }
        }
    }

    void Dump() {
        for (const auto& line : text_) {
            std::cout << line << '\n';
        }
    }
private:
    void ApplyUp() {
        if (cursor_ == text_.begin()) {
            return;
        }
        cursor_ = std::prev(cursor_);
    }
    void ApplyDown() {
        auto next = std::next(cursor_);
        if (next == text_.end()) {
            return;
        }
        cursor_ = next;
    }
    void ApplyCut() {
        if (cursor_->size() == 0) {
            return;
        }
        auto prev = cursor_;
        cursor_ = std::next(cursor_);
        buffer_ = std::move(*prev);
        text_.erase(prev);
    }
    void ApplyInsert() {
        if (buffer_.empty()) {
            return;
        }
        text_.insert(cursor_, buffer_);
    }
private:
    std::list<std::string> text_;
    std::list<std::string>::iterator cursor_;
    std::string buffer_;
};


int main() {
    Editor editor;
    editor.ReadCommands();
    editor.Dump();
    return 0;
}