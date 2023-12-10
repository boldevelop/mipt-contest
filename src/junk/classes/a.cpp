#include <iostream>

class Date {
    bool isValidDay(int d) {
        return d > 0 && d < 32;
    }
    bool isValidMonth(int m) {
        return m > 0 && m < 13;
    }
    bool isValidYear(int y) {
        return y > 1969 && y < 2100;
    }
    void initDefault() {
        d_ = 1;
        m_ = 1;
        y_ = 1970;
    }
public:
    Date(int d, int m, int y) {
        if (!isValidDay(d) ||
            !isValidMonth(m) || 
            !isValidYear(y)) {
            initDefault();
            return;
        }
        d_ = d;
        m_ = m;
        y_ = y;
    }

    int GetDay() const {
        return d_;
    }
    int GetMonth() const {
        return m_;
    }
    int GetYear() const {
        return y_;
    }
    int Days() const {
        return d_ + m_ * 31 + 12 * 31 * y_;
    }
private:
    int d_;
    int m_;
    int y_;
};

Date getDateFromDays(int d) {
    auto days_y = 12 * 31;
    auto y = d / days_y;
    d = d - y * days_y;
    if (d == 0) {
        return Date{1, 1, y};
    }

    auto days_m = 31;
    auto m = d / days_m;
    d = d - m * days_m;
    if (d == 0) {
        return Date{1, m, y};
    }
    return Date{d, m, y};
}

Date operator-(const Date& l, int days) {
    return getDateFromDays(l.Days() - days);
}

Date operator+(const Date& l, int days) {
    return getDateFromDays(l.Days() + days);
}

int operator-(const Date& l, const Date& r) {
    return l.Days() - r.Days();
}

// std::ostream& operator<<(std::ostream& out, const Date& d) {
//     out << d.GetDay() << " " << d.GetMonth() << " " << d.GetYear();
//     return out;
// }

// int main() {
//     Date d1{20, 1, 2003};
//     Date d2{18, 1, 2003};
//     std::cout << d1 + 3 << '\n';
//     std::cout << d1 - 3 << '\n';
//     std::cout << d1 - d2 << '\n';
// }
