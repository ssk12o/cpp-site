#include "utils.hpp"

#include <cassert>
#include <ctime>
#include <iostream>

static int test_counter = 1;

void run_test(void (*fn)()) {
    std::cout << "Test no. " << test_counter++ << ": ";
    fn();
    std::cout << "OK" << std::endl;
}

void test_random_name_not_empty() {
    std::string name = utils::random_name();
    assert(name.size() > 0);
}

void test_random_name_starts_uppercase() {
    std::string name = utils::random_name();
    assert(name[0] >= 'A' && name[0] <= 'Z');
}


int main() {
    std::srand(std::time(nullptr));
    run_test(test_random_name_not_empty);
    run_test(test_random_name_starts_uppercase);
    return 0;
}
