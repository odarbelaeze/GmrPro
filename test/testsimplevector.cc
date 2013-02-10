#include <cassert>
#include "../simplevector.h"

#define ASSERT(counter, condition) assert(condition); \
    std::cout << ++counter << " Tests passed." << std::endl;

#define EXPECT_EXEPTION(counter, command, exeption) try     \
    {                                                       \
        command;                                            \
    }                                                       \
    catch (exeption e)                                      \
    {                                                       \
        ASSERT(counter, true)                               \
    }                                                       \

int main(int argc, char const *argv[])
{
    Vector u(1, 0, 0);
    Vector v(0, 1, 0);
    Vector w;

    int passed = 0;

    ASSERT(passed, dot(u, v) == 0)
    ASSERT(passed, norm(u) == 1)
    ASSERT(passed, norm(v) == 1)
    ASSERT(passed, norm(u + v) == norm(u - v))
    ASSERT(passed, u != v)
    ASSERT(passed, u * v == w)
    ASSERT(passed, u * 1 == u)
    ASSERT(passed, 1 * u == u)
    ASSERT(passed, (u / 2) * 2 == u)
    ASSERT(passed, (u * 0) == Vector())
    ASSERT(passed, (u + v) == Vector(1, 1, 0))
    ASSERT(passed, (u - v) == Vector(1, -1, 0))
    ASSERT(passed, -u == Vector(-1, 0, 0))
    ASSERT(passed, u(0) == u[0])

    u(0) = 5;
    u[0] = 5;

    ASSERT(passed, u(0) == 5)
    ASSERT(passed, u[0] == 5)

    u = v;
    ASSERT(passed, u == v)

    EXPECT_EXEPTION(passed, u(5) = 0, OutOfBoundsException)

    return 0;
}
