#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>

namespace person {

    /**
     * Person entity, attributed with name and age.
     * Non-copyable.
     */
    class Person {
        std::string _name;
        int _age;

    public:
         Person(std::string name, int age) : _name(name), _age(age) {}

         Person(const Person& other) = delete;

         Person& operator=(const Person& other) = delete;

         const std::string& name() const { return _name; }

         int age() const { return _age; }
    };

}

#endif
