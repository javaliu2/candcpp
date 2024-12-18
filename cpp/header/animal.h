#ifndef ANIMAL_H_
#define ANIMAL_H_
class Animal{
    private:
        char* name;
        int age;
    public:
        Animal();
        Animal(const char* name, int age);
        Animal(const Animal& obj);
        ~Animal();
        Animal& operator=(const Animal& obj);
        virtual void say();
};
#endif
