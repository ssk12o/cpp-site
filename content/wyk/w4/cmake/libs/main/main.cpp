#include "generator.hpp"

#include <iostream>
#include <ctime>

int main() {
    std::srand(std::time(nullptr));
    person::Person p = generator::generate();
    std::cout << "Hello " << p.name() << "!" << std::endl;
    return 0;
}
