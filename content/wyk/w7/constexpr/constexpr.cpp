constexpr int add(int x, int y) { return x + y; }

int main(int argc, char** argv)
{
    constexpr int x = 1;
    constexpr int y = 2;
    constexpr int result = 2 * x + y;

    constexpr int limit = add(result, x);

    if (argc > limit) {
        return 1;
    }

    return 0;
}
