#include <iostream>
#include <list>
#include <iterator>
#include <string>

class Editor {
    typedef std::list<std::string>::iterator Iter;
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
        ClearShift();
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
            } else if (line == "Shift") {
                ApplyShift();
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
        MoveCursorUp();
    }
    void ApplyDown() {
        auto next = std::next(cursor_);
        if (next == text_.end()) {
            return;
        }
        MoveCursorDown();
    }
    void ApplyCut() {
        if (cursor_->size() == 0) {
            return;
        }
        Iter start, end;
        if (shift_enable_ && shift_lines_ != 0) {
            start = shift_lines_ > 0 ? cursor_ : shift_cursor_start_; 
            end = shift_lines_ > 0 ? shift_cursor_start_ : cursor_;
        } else {
            start = cursor_;
            end = std::next(start);
        }

        buffer_.clear();
        buffer_.splice(buffer_.begin(), text_, start, end);
        cursor_ = end;

        ClearShift();
    }
    void ApplyInsert() {
        if (buffer_.empty()) {
            return;
        }
        if (shift_enable_ && shift_lines_ != 0) {
            auto start = shift_lines_ > 0 ? cursor_ : shift_cursor_start_; 
            auto end = shift_lines_ > 0 ? shift_cursor_start_ : cursor_;
            cursor_ = text_.erase(start, end);
        }
        text_.splice(cursor_, std::list(buffer_));
        ClearShift();
    }
    void ApplyShift() {
        shift_enable_ = true;
        shift_lines_ = 0;
    }

    void ClearShift() {
        shift_enable_ = false;
        shift_lines_ = 0;
    }
    void MoveCursorDown() {
        auto next = std::next(cursor_);
        if (shift_enable_) {
            if (shift_lines_ == 0) {
                shift_cursor_start_ = cursor_;
            }
            shift_lines_--;
        }
        cursor_ = next;
    }
    void MoveCursorUp() {
        if (shift_enable_) {
            if (shift_lines_ == 0) {
                shift_cursor_start_ = cursor_;
            }
            shift_lines_++;
        }
        cursor_ = std::prev(cursor_);
    }
private:
    std::list<std::string> text_;
    Iter cursor_;
    
    std::list<std::string> buffer_;
    bool shift_enable_;
    Iter shift_cursor_start_;
    int shift_lines_;
};


int main() {
    Editor editor;
    editor.ReadCommands();
    editor.Dump();
    return 0;
}