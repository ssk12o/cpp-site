#include "utils.hpp"
#include "impl.hpp"

#include <vector>
#include <string>
#include <ctime>

namespace utils {

std::string random_name() {
    // Generate a random index and return the corresponding name
    int randomIndex = std::rand() % names.size();
    return names[randomIndex];
}

}
