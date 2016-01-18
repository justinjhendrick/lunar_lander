#ifndef PILOT_H_
#define PILOT_H_

#include "Ground.hpp"
#include "World.hpp"
#include "Lander.hpp"

class Pilot : Lander {
    private:
    public:
        Pilot(Screen& s);
        void fly(World& world);
};

#endif
