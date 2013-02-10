#include "GmrPro.h"

int main(int argc, char const *argv[])
{
    System system;
    if (argc == 1)
    {
        system = System(std::cin);
    }
    else
    {
        system = System(argv[1]);
    }

    

    return 0;
}