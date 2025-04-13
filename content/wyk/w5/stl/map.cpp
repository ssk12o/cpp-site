#include <iostream>
#include <map>

struct color
{
    int r, g, b;

    color() : r(0), g(0), b(0)
    {
        std::cout << "color()" << std::endl;
    }

    color(int r, int g, int b) : r(r), g(g), b(b)
    {
        std::cout << "color(" << r << ", " << g << ", " << b << ")" << std::endl;
    }

    ~color()
    {
        std::cout << "~color() (" << r << ", " << g << ", " << b << ")" << std::endl;
    }

    color(const color& c) : r(c.r), g(c.g), b(c.b)
    {
        std::cout << "color(const color &c) (" << r << ", " << g << ", " << b << ")" << std::endl;
    }

    color(color&& c) noexcept : r(c.r), g(c.g), b(c.b)
    {
        std::cout << "color(color &&c) (" << r << ", " << g << ", " << b << ")" << std::endl;
    }

    color& operator=(const color& c)
    {
        r = c.r;
        g = c.g;
        b = c.b;
        std::cout << "color &operator=(const color &c) (" << r << ", " << g << ", " << b << ")" << std::endl;
        return *this;
    }

    color& operator=(color&& c) noexcept
    {
        r = c.r;
        g = c.g;
        b = c.b;
        std::cout << "color &operator=(color &&c) (" << r << ", " << g << ", " << b << ")" << std::endl;
        return *this;
    }
};

int main()
{
    std::map<std::string, color> colors;
    std::cout << "  emplace red" << std::endl;
    colors.emplace(std::piecewise_construct, std::forward_as_tuple("red"), std::forward_as_tuple(255, 0, 0));
    std::cout << "  insert green" << std::endl;
    colors.insert(std::make_pair("green", color{0, 255, 0}));
    std::cout << "  colors[blue]" << std::endl;
    colors["blue"] = color{0, 0, 255};
    std::cout << "  clear" << std::endl;
    colors.clear();

    return 0;
}
