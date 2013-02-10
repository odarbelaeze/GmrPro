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

    // ASSERT(counter, particle.getPosition() == Vector(5))




    Particle otro("Electrón");
    std::cout << otro.getType() << std::endl;

    otro.setPosition(Vector(0,0,0));
    std::cout << otro.getPosition() << std::endl;

    Vector v_template;
    v_template.setX(5);
    v_template.setY(10);
    v_template.setZ(15);
    otro.setPosition(v_template);
    std::cout << otro.getPosition() << std::endl;

    return 0;
}