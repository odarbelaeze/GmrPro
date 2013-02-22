#include <cassert>
#include "particle.h"
#include "system.h"


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
    srand48(time(NULL));

    // Particle otro("Electrón");
    // std::cout << "type: " << otro.getType() << std::endl;
    // std::cout << "position: " << otro.getPosition() << std::endl;
    // std::cout << "spin: " << otro.getSpin() << std::endl;
    // std::cout << "oldSpin: " << otro.getOldSpin() << std::endl;
    // std::cout << "id: " << otro.getId() << std::endl;
    // std::cout << "charge: " << otro.getCharge() << std::endl;

    // otro.setPosition(Vector(5));
    // otro.setOldPosition(otro.getPosition());
    // otro.setOldSpin(otro.getSpin());
    // otro.setPosition(randomVector());
    // otro.setSpin(randomVector());
    // otro.setId(5);
    // otro.setType("protonión");
    // otro.setCharge(-500);

    // std::cout << std::endl << "type: " << otro.getType() << std::endl;
    // std::cout << "position: " << otro.getPosition() << std::endl;
    // std::cout << "spin: " << otro.getSpin() << std::endl;
    // std::cout << "oldSpin: " << otro.getOldSpin() << std::endl;
    // std::cout << "id: " << otro.getId() << std::endl;
    // std::cout << "charge: " << otro.getCharge() << std::endl;

    // Vector A(Vector(4));
    // Vector B(Vector(3));
    // Vector C(Vector(12));
    // ASSERT(counter, A*B == C)

    System sistemita();

    return 0;
}