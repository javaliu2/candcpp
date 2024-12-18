#ifndef FRACTION_H
#define FRACTION_H
#include <string>
#include "student.h"
#include <iostream>

class Fraction{
    private:
        int numerator;
        int denominator;
    public:
        Fraction();
        Fraction(int a, int b);
        // Fraction(const std::string& str);  // other class ==> this class
        explicit Fraction(const std::string& str);  // 
        // this class ==> other class
        operator double() const;  // to double
        operator std::string() const; // to string
        operator Student() const; // to user-defined class
        friend std::ostream& operator<<(std::ostream& out, const Fraction& o);
};

#endif