#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include "Lander.hpp"
#include "Screen.hpp"
#include "constants.hpp"
#include "Utils.hpp"

Lander::Lander(Screen& s) :
    Lander(s,
           Screen::WIDTH / 2 - WIDTH / 2,  // x_pos
           Screen::HEIGHT / 4, // y_pos
           0.,     // x_vel
           0.,     // y_vel
           3 * M_PI_2,     // orientation
           0.,     // spin_rate
           .08,    // max_torque
           2150.,  // dry_mass
           400.,   //2353.,  // init_fuel
           0.,     // thrust
           30000., //16000., // max_thrust
           3050.   // exhaust velocity
    ) {
    // values above from the Apollo lunar module
    // https://en.wikipedia.org/wiki/Apollo_Lunar_Module
}

Lander::Lander(Screen& s,
       float _x_pos,
       float _y_pos,
       float _x_vel,
       float _y_vel,
       float _orientation,
       float _spin_rate,
       float _max_torque,
       float _dry_mass,
       float _init_fuel,
       float _thrust,
       float _max_thrust,
       float _exhaust_vel) {
    // set init vals
    x_pos = _x_pos;
    y_pos = _y_pos;
    x_vel = _x_vel;
    y_vel = _y_vel;
    orientation = _orientation;
    spin_rate = _spin_rate;
    max_torque = _max_torque;
    fuel = _init_fuel;
    dry_mass = _dry_mass;
    init_fuel = _init_fuel;
    thrust = _thrust;
    max_thrust = _max_thrust;
    exhaust_vel = _exhaust_vel;

    torque = 0.;
    dt = FRAME_TIME / 1000.;
    txtr = Utils::load_texture(s.renderer, "sprites/lander.bmp");
    txtr_fire_low = Utils::load_texture(s.renderer, "sprites/lander_fire_low.bmp");
    txtr_fire_med = Utils::load_texture(s.renderer, "sprites/lander_fire_med.bmp");
    txtr_fire_high = Utils::load_texture(s.renderer, "sprites/lander_fire_high.bmp");
    fuel_txtr = Utils::create_text_texture(s, "FUEL ");
    thrust_txtr = Utils::create_text_texture(s, "THRUST ");
    vel_txtr = Utils::create_text_texture(s, "VEL ");
}

void Lander::handle(SDL_Event* e) {
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_UP:
                thrust += max_thrust / 10;
                if (thrust > max_thrust) {
                    thrust = max_thrust;
                }
                break;
            case SDLK_DOWN:
                thrust -= max_thrust / 10;
                if (thrust < 0) {
                    thrust = 0;
                }
                break;
            case SDLK_LEFT:
                torque = -max_torque;
                break;
            case SDLK_RIGHT:
                torque = max_torque;
                break;
            case SDLK_SPACE:
                if (fuel > 0.) {
                    thrusting = true;
                }
                break;
            default:
                break;
        }
    } else if (e->type == SDL_KEYUP) {
        switch (e->key.keysym.sym) {
            case SDLK_LEFT:
                torque = 0.;
                break;
            case SDLK_RIGHT:
                torque = 0.;
                break;
            case SDLK_SPACE:
                thrusting = false;
                break;
            default:
                break;
        }
    }
}

void Lander::craft_to_sdl_coords() {
    float s = sin(3 * M_PI_2 + orientation);
    float c = cos(3 * M_PI_2 + orientation);
    p1x = c * sc_p1x - s * sc_p1y + WIDTH / 2 + x_pos;
    p1y = s * sc_p1x + c * sc_p1y + COLLISION_HEIGHT + y_pos;

    p2x = c * sc_p2x - s * sc_p2y + WIDTH / 2 + x_pos;
    p2y = s * sc_p2x + c * sc_p2y + COLLISION_HEIGHT + y_pos;

    p3x = c * sc_p3x - s * sc_p3y + WIDTH / 2 + x_pos;
    p3y = s * sc_p3x + c * sc_p3y + COLLISION_HEIGHT + y_pos;
}

void Lander::move() {
    spin_rate += torque;
    orientation += spin_rate;
    if (orientation > 2 * M_PI) {
        orientation -= 2 * M_PI;
    } else if (orientation < 0) {
        orientation += 2 * M_PI;
    }

    float x_accel = 0.;
    float y_accel = 0.;
    if (thrusting && thrust > 0.) {
        // compute acceleration
        x_accel = thrust * cos(orientation) / (dry_mass + fuel);
        y_accel += thrust * sin(orientation) / (dry_mass + fuel);

        float new_x_vel = x_vel + x_accel * dt;
        float new_y_vel = y_vel + y_accel * dt;
        
        // calculate mass change
        float dmdt = ((dry_mass + fuel) * sqrt(pow(new_x_vel - x_vel, 2) +
                     pow(new_y_vel - y_vel, 2) / dt) - thrust) / exhaust_vel;
        fuel += dmdt * dt;
    }
    x_vel += x_accel;
    y_vel += y_accel + 1.62; // gravity

    vel = sqrt(x_vel * x_vel + y_vel * y_vel);

    // calculate new position
    x_pos += x_vel * dt + .5 * x_accel * dt * dt;
    y_pos += y_vel * dt + .5 * y_accel * dt * dt;

    // recalculate collision points
    craft_to_sdl_coords();
}

void Lander::draw_status(Screen& s) {
    int bar_width = 100;

    // the box where the thrust level bar goes
    int thrust_text_width = 42;
    SDL_Rect thrust_bar;
    thrust_bar.x = Screen::WIDTH / 4 -
            (thrust_text_width + bar_width) / 2 +
            thrust_text_width;
    thrust_bar.y = 13;
    thrust_bar.w = bar_width;
    thrust_bar.h = 10;

    // the box where the text "thrust" goes
    SDL_Rect thrust_text;
    thrust_text.x = Screen::WIDTH / 4 - (thrust_text_width + bar_width) / 2;
    thrust_text.y = 10;
    thrust_text.w = thrust_text_width;
    thrust_text.h = 16;

    // the box there velocity goes
    int vel_text_width = 24;
    SDL_Rect vel_bar;
    vel_bar.x = Screen::WIDTH / 2 -
            (vel_text_width + bar_width) / 2 +
            vel_text_width;
    vel_bar.y = 13;
    vel_bar.w = bar_width;
    vel_bar.h = 10;

    // the box where the text "vel" goes
    SDL_Rect vel_text;
    vel_text.x = Screen::WIDTH / 2 - (vel_text_width + bar_width) / 2;
    vel_text.y = 10;
    vel_text.w = vel_text_width;
    vel_text.h = 16;

    // the box where the fuel level bar goes
    int fuel_text_width = 30;
    SDL_Rect fuel_bar;
    fuel_bar.x = 3 * Screen::WIDTH / 4 -
            (fuel_text_width + bar_width) / 2 +
            fuel_text_width;
    fuel_bar.y = 13;
    fuel_bar.w = bar_width;
    fuel_bar.h = 10;

    // the box where the text "fuel" goes
    SDL_Rect fuel_text;
    fuel_text.x = 3 * Screen::WIDTH / 4 - (fuel_text_width + bar_width) / 2;
    fuel_text.y = 10;
    fuel_text.w = fuel_text_width;
    fuel_text.h = 16;

    // write words
    SDL_RenderCopy(s.renderer, thrust_txtr, NULL, &thrust_text);
    SDL_RenderCopy(s.renderer, vel_txtr, NULL, &vel_text);
    SDL_RenderCopy(s.renderer, fuel_txtr, NULL, &fuel_text);

    // save old color
    Uint8 old_r, old_g, old_b, old_a;
    SDL_GetRenderDrawColor(s.renderer, &old_r, &old_g, &old_b, &old_a);
    SDL_SetRenderDrawColor(s.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // thrust bar
    SDL_RenderDrawRect(s.renderer, &thrust_bar);
    thrust_bar.w = (int) (thrust * ((float) bar_width) / max_thrust);
    SDL_RenderFillRect(s.renderer, &thrust_bar);

    // fuel bar
    SDL_RenderDrawRect(s.renderer, &fuel_bar);
    fuel_bar.w = (int) (fuel * ((float) bar_width) / init_fuel);
    SDL_RenderFillRect(s.renderer, &fuel_bar);

    // velocity bar
    SDL_RenderDrawRect(s.renderer, &vel_bar);
    vel_bar.w = (int) (vel * ((float) bar_width) / max_vel);
    if (vel_bar.w > bar_width) {
        vel_bar.w = bar_width; // don't go past end of bar
    }

    // if we're in the safe zone, make bar green
    if (vel <= safe_vel) {
        SDL_SetRenderDrawColor(s.renderer, 0x55, 0xFF, 0x55, 0xFF);
    }
    SDL_RenderFillRect(s.renderer, &vel_bar);

    // outline safe zone in green
    SDL_SetRenderDrawColor(s.renderer, 0x55, 0xFF, 0x55, 0xFF);
    vel_bar.w = (int) (safe_vel * ((float) bar_width) / max_vel);
    SDL_RenderDrawRect(s.renderer, &vel_bar);

    // reset old color
    SDL_SetRenderDrawColor(s.renderer, old_r, old_g, old_b, old_a);
}

void Lander::draw(Screen& s) {
    draw_status(s);

    // draw lander itself
    SDL_Rect dest;
    dest.x = (int) x_pos;
    dest.y = (int) y_pos;
    dest.w = WIDTH;
    dest.h = HEIGHT;

    // choose texture based on thrust level
    SDL_Texture* t = txtr;
    if (thrusting && thrust > 0.) {
        if (thrust < max_thrust / 3) {
            t = txtr_fire_low;
        } else if (thrust < 2 * max_thrust / 3) {
            t = txtr_fire_med;
        } else {
            t = txtr_fire_high;
        }
    }

    // rotate about middle of thruster (at bottom of craft)
    SDL_Point rot_abt;
    rot_abt.x = WIDTH / 2;
    rot_abt.y = COLLISION_HEIGHT;
    // draw to dest, rotated by orientation
    SDL_RenderCopyEx(s.renderer,
                     t,
                     NULL,
                     &dest,
                     (M_PI_2 + orientation) * 180. / M_PI,
                     &rot_abt,
                     SDL_FLIP_NONE
    );
}

bool Lander::safe_speed() {
    return vel < safe_vel;
}

Lander::~Lander() {
    SDL_DestroyTexture(txtr);
    SDL_DestroyTexture(txtr_fire_low);
    SDL_DestroyTexture(txtr_fire_med);
    SDL_DestroyTexture(txtr_fire_high);
    SDL_DestroyTexture(fuel_txtr);
    SDL_DestroyTexture(thrust_txtr);
    SDL_DestroyTexture(vel_txtr);
}
