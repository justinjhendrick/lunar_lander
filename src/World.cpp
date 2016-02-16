#include "World.hpp"
#include "Utils.hpp"

World::World () {
    int pad_x = Utils::rand_int(0, Screen::WIDTH - SEGMENT_WIDTH);
    int pad_y = Utils::rand_int(3 * Screen::HEIGHT / 4, Screen::HEIGHT - 10);
    Ground pad(true, pad_x, pad_y,
                     pad_x + SEGMENT_WIDTH, pad_y);
    Ground top(false, 0, 0, Screen::WIDTH, 0);
    Ground left(false, 0, 0, 0, Screen::HEIGHT);
    Ground right(false, Screen::WIDTH, 0, Screen::WIDTH, Screen::HEIGHT);
    grounds.push_back(pad);
    grounds.push_back(top);
    grounds.push_back(left);
    grounds.push_back(right);
    
    // The pad location has already been chosen,
    // create random terrain on both sides of it
    generate_terrain(pad_x, pad_y, -SEGMENT_WIDTH);
    generate_terrain(pad_x + SEGMENT_WIDTH, pad_y, SEGMENT_WIDTH);
}

// generate random terrain from (init_x, init_y) until the wall by dx steps
//
// Start from the pad and go to the wall because that's much easier than
// taking random steps and hitting a target. If we start at the pad,
// all we have to do is hit the wall somewhere to make contiguous terrain
void World::generate_terrain(int init_x, int init_y, int dx) {
    int last_x = init_x;
    int last_y = init_y;
    int this_x;
    int this_y;
    while (last_x >= 0 && last_x < Screen::WIDTH) {
        int dy = Utils::rand_int(-MAX_DY, MAX_DY);

        // if we would go off the bottom (or top) of screen,
        // just go the other way
        if (last_y + dy < 0 || last_y + dy >= Screen::HEIGHT) {
            dy *= -1;
        }

        this_x = last_x + dx;
        this_y = last_y + dy;

        // these cases are needed because Ground() requires x1 <= x2
        if (dx < 0) {
            // going left
            Ground next(false, this_x, this_y, last_x, last_y);
            grounds.push_back(next);
        } else {
            // going right
            Ground next(false, last_x, last_y, this_x, this_y);
            grounds.push_back(next);
        }

        last_x = this_x;
        last_y = this_y;
    }
}

void World::draw(Screen& s) {
    for (unsigned int i = 0; i < grounds.size(); i++) {
        grounds[i].draw(s);
    }
}

Ground& World::get_pad() {
    return grounds[0];
}
