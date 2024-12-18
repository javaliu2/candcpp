#include "animal.h"
#include <iostream>
#include <cstring>
using namespace std;

Animal::Animal() : name(nullptr), age(0) {
    cout << "Anima::Animal()" << endl;
}

Animal::Animal(const char* name, int age) : age(age){
    cout << "Anima::Animal(parameters)" << endl;
    size_t len = strlen(name) + 1;
    this->name = new char[len];
    strcpy(this->name, name);
}

Animal::Animal(const Animal& obj) : age(obj.age){
    cout << "Anima::Animal(const Animal&)" << endl;
    size_t len = strlen(obj.name) + 1;
    name = new char[len];
    strcpy(name, obj.name);
}

Animal::~Animal()
{
    cout << "Animal::~Animal()" << endl;
    delete[] name;
  
}

Animal& Animal::operator=(const Animal& obj) 
{
    cout << "Animal::operator=()" << endl;
    if (this == &obj) {
        return *this;
    }
    delete[] name;
    size_t len = strlen(obj.name) + 1;
    name = new char[len];
    strcpy(name, obj.name);
    age = obj.age;
    return *this;
}

void Animal::say()
{
    cout << "Based Class: Animal. " << "name: " << name << "; age: " << age << endl;
}
