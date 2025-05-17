#include <iostream>

constexpr bool is_prime(int x) {
    if (x <= 1) return false;
    for (int i = 2; i * i < x; ++i) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

constexpr int nth_prime(int n) {
    for (int i = 2;; ++i) {
        if (is_prime(i)) {
            n--;
            if (n == 0) return i;
        }
    }
}


constexpr int fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}



int main(int argc, char** argv)
{
    std::cout << nth_prime(1000) << std::endl;
    return 0;
}
