#include "Player.hpp"

Player::Player(bool human, Colors::PlayerColor _color) {
    if (human) {
        pilot = nullptr;
    } else {
        pilot = new Pilot();
    }
    color = _color;
    score = 0;
}

bool Player::is_human() {
    return pilot == nullptr;
}

void Player::prep_for_next_round() {
    if (pilot != nullptr) {
        delete pilot;
        pilot = new Pilot();
    }
}

Player::~Player() {
    if (pilot != nullptr) {
        delete pilot;
    }
}
