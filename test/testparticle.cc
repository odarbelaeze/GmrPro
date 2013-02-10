#include <cassert>
#include "../particle.h"
#include "../system.h"


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
    int counter = 0;
    // ASSERT(counter, particle.getPosition() == Vector(5))

    Particle otro("Electrón");
    std::cout << "type: " << otro.getType() << std::endl;
    std::cout << "position: " << otro.getPosition() << std::endl;
    std::cout << "spin: " << otro.getSpin() << std::endl;
    std::cout << "oldSpin: " << otro.getOldSpin() << std::endl;
    std::cout << "id: " << otro.getId() << std::endl;
    std::cout << "charge: " << otro.getCharge() << std::endl;

    otro.setPosition(Vector(5));
    otro.setSpin(randomVector());
    otro.setOldPosition(otro.getPosition());


    return 0;
}