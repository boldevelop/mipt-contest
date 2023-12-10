#include <iostream>
#include <list>
#include <deque>
#include <string>
#include <string_view>
#include <cstdlib>
#include <iterator>

/* 
1) 3 1
2) 1 5 6 7
3) 4 5 6 7
4) 7 6 5 4
5) 7 6 5 1
6) 5 6 7
7) 5 6 7
8) 5 6 7 8
 */
class Train {
public:
    Train(): wagons_(), left_(0), right_(0) {
    }

    void AddLeft(int w_n) {
        if (wagons_.empty()) {
            return AddToEmpty(w_n);
        }
        if (isOnLeftBorder()) {
            wagons_.push_front(w_n);
            right_++;
            return;
        }
        left_--;
        wagons_[left_] = w_n;
    }

    void AddRight(int w_n) {
        if (wagons_.empty()) {
            return AddToEmpty(w_n);
        }
        if (isOnRightBorder()) {
            wagons_.push_back(w_n);
            right_++;
            return;
        }
        wagons_[right_] = w_n;
        right_++;
    }

    void RemoveLeft(int count) {
        auto size = RealSize();
        if (size == 0) {
            return;
        }
        if (size <= count) {
            Clear();
            return;
        }
        left_ += count;
    }

    void RemoveRight(int count) {
        auto size = RealSize();
        if (size == 0) {
            return;
        }
        if (size <= count) {
            Clear();
            return;
        }
        right_ -= count;
    }

    void Dump() {
        auto left = left_;
        while (left < right_) {
            std::cout << wagons_[left++] << " ";
        }
        std::cout << '\n';
    }
private:
    void AddToEmpty(int w_n) {
        wagons_.push_back(w_n);
        left_ = 0;
        right_ = wagons_.size();
    }

    bool isEmpty() {
        return left_ == right_;
    }
    bool isOnLeftBorder() {
        return left_ == 0;
    }
    bool isOnRightBorder() {
        return right_ == static_cast<int>(wagons_.size());
    }
    int RealSize() {
        return right_ - left_;
    }
    void Clear() {
        left_ = right_ = wagons_.size() > 2 ? wagons_.size() / 2 : 0;
    }

private:
    std::deque<int> wagons_;
    int left_;
    int right_;
};

void MakeTrain() {
    std::list<int> w;
    std::string command;
    Train train;
    while (std::getline(std::cin, command)) {
        auto space_pos = command.find(' ');
        auto num = std::stoi(std::string(command, space_pos + 1, command.size() - space_pos - 1)/* command.c_str() + space_pos + 1 */);
        bool isAdd = command[0] == '+';
        bool right = command[1] == 'r';

        if (isAdd) {
            if (right) {
                train.AddRight(num);
                continue;
            }
            train.AddLeft(num);
        } else {
            if (right) {
                train.RemoveRight(num);
                continue;
            }
            train.RemoveLeft(num);
        }
    }

    train.Dump();
}

int main() {
    MakeTrain();
    return 0;
}