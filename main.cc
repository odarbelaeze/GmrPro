#include "helpers.h"

int main(int argc, char const *argv[])
{
    darray a { 10, 20, 30 };
    std::cout << a[std::slice(1, 2, - 1)] << std::endl;

    darray b { 10, 20, 30, 40, 50, 60, 70, 80 };

    std::cout << b[std::gslice(1, {2, 1}, { 1, 2 })] << std::endl;


    std::valarray<size_t> sel {1, 2, 3};
    std::cout << b[sel] << std::endl;

    return 0;
}
