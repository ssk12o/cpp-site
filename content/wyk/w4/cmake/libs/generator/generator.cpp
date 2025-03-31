#include "generator.hpp"

#include "utils.hpp"

namespace generator {

    person::Person generate() {
           return person::Person(utils::random_name(), 22);
    }

}
