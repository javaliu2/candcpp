#ifndef CAT_H_
#define CAT_H_
#include "animal.h"

class Cat : public Animal {
    private:
        int secret;
    public:
        Cat();
        Cat(const char* name, int age, int secret);
        Cat(const Cat& cat);
        ~Cat();
        Cat& operator=(const Cat& cat);
        virtual void say();
};

#endif