#ifndef PILOT_H_
#define PILOT_H_

#include "World.hpp"
#include "Lander.hpp"

class Pilot {
    private:
    public:
        Pilot();
        void fly(Lander& l, World& world);
};

#endif
