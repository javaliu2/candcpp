#include "fraction.h"
#include "student.h"

Fraction::Fraction() : numerator(0), denominator(0) {}

Fraction::Fraction(int a, int b) {
    numerator = a;
    denominator = b;
}

Fraction::Fraction(const std::string &str) {
    // 以下两行是拿到字符所表示的int值，而不是将字符转化为int
    // numerator = int(str[0]);
    // denominator = int(str[1]);
    numerator = str[0] - '0';
    denominator = str[1] - '0';
}

Fraction::operator double() const {
    return numerator * 1.0 / denominator;
}

Fraction::operator std::string() const {
    return to_string(numerator) + "/" + to_string(denominator);
}

Fraction::operator Student() const {
    return Student("xs", 24, "time");
}

std::ostream& operator<<(std::ostream& out, const Fraction& o){
    cout << o.numerator << "/" << o.denominator << endl;
    return out;
}
