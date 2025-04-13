#ifndef INT_HPP
#define INT_HPP

#include <iostream>

class A {
public:
    A() { std::cout << "A()\n"; }
    ~A() { std::cout << "~A()\n"; }
};

class Int
{
    int val;
public:
    Int(int val) : val{val} { std::cout << "Int()" << " val = " << val << "\n"; }
    Int(const Int& i) : val{i.val} { std::cout << "Int(const Int&)" << " val = " << val << "\n"; }
    Int(Int&& i): val{i.val} { std::cout << "Int(Int&&)" << " val = " << val << "\n"; }
    Int& operator=(const Int& i) { val = i.val; std::cout << "Int& operator=(const Int&)" << " val = " << val << "\n"; return *this; }
    Int& operator=(Int&& i) {  val = i.val; std::cout << "Int& operator=(Int&&)" << " val = " << val << "\n"; return *this; }
    ~Int() { std::cout << "~Int()" << " val = " << val << "\n"; }
};

#endif
