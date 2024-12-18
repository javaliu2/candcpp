#include "cat.h"
#include <iostream>
using namespace std;

Cat::Cat() : secret(0) {
    cout << "Cat::Cat()" << endl;
}

Cat::Cat(const char *name, int age, int secret) : Animal(name, age), secret(secret)
{
    cout << "Cat::Cat(parameters)" << endl;
}


Cat::Cat(const Cat &cat) : Animal(cat), secret(cat.secret)
{
    cout << "Cat::Cat(const Cat&)" << endl;
}

Cat::~Cat()
{
    cout << "Cat::~Cat()" << endl;   
}

Cat& Cat::operator=(const Cat &cat)
{
    cout << "Cat::operator=()" << endl;
    if (this == &cat) {
        return *this;
    }
    Animal::operator=(cat);
    secret = cat.secret;
    return *this;
}

void Cat::say()
{
    Animal::say();
    cout << "Derived Class: Cat" << ", secret: " << secret << endl;
}
