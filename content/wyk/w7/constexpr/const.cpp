int add(int x, int y) { return x + y; }

int main(int argc, char** argv)
{
    const int x = 1;
    const int y = 2;
    const int result = 2 * x + y;

    int limit = add(result, x);

    if (argc > limit) {
        return 1;
    }

    return 0;
}
