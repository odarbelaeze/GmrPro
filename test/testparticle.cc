#include <cassert>
#include "../particle.h"

#define ASSERT(counter, condition) assert(condition);       \
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
    Particle particle;
    Vector vector(1.0);
    int counter = 0;

    particle.setPosition(Vector(5));

    ASSERT(counter, particle.getPosition() == Vector(5))

    return 0;
}