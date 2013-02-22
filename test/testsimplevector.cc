#include <cassert>
#include "simplevector.h"

#define ASSERT(counter, condition) assert(condition);       \
    std::cout << ++counter << " Tests passed." << std::endl;

#define EXPECT_EXEPTION(counter, command, exeption) try     \
    {                                                       \
        command;                                            \
    }                                                       \
    catch (exeption e)                                      \
    {                                                       \
        ASSERT(counter, true)                               \
    }                                                       

int main(int argc, char const *argv[])
{
    Vector dimensions(10, 10, 10);
    Vector boundaryConditions(1, 1, 1);
    Vector a(9.5, 5, 5);
    Vector b(0.5, 5, 5);

    std::cout << distancePbc(a, b, dimensions, boundaryConditions) << std::endl;
    
    return 0;
}
