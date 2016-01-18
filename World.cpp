#include "World.hpp"

World::World () {
    Ground pad(true, Screen::WIDTH / 2 - 20, Screen::HEIGHT - 10,
                     Screen::WIDTH / 2 + 20, Screen::HEIGHT - 10);
    Ground top(false, 0, 0, Screen::WIDTH, 0);
    Ground left(false, 0, 0, 0, Screen::HEIGHT);
    Ground right(false, Screen::WIDTH, 0, Screen::WIDTH, Screen::HEIGHT);
    Ground bot(false, 0, Screen::HEIGHT, Screen::WIDTH, Screen::HEIGHT);
    grounds.push_back(pad);
    grounds.push_back(top);
    grounds.push_back(left);
    grounds.push_back(right);
    grounds.push_back(bot);
}

void World::draw(Screen& s) {
    for (int i = 0; i < grounds.size(); i++) {
        grounds[i].draw(s);
    }
}
