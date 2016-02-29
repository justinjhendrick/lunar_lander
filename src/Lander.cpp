#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include "Lander.hpp"
#include "Screen.hpp"
#include "constants.hpp"
#include "Utils.hpp"
#include "World.hpp"

Lander::Lander(Screen* s) :
    Lander(s,
           Screen::WIDTH / 2 - WIDTH / 2,  // x_pos
           Screen::HEIGHT / 4,             // y_pos
           Utils::rand_double(-5., 5.),    // x_vel
           0.,                             // y_vel
           3 * M_PI_2,                     // orientation
           0.,                             // spin_rate
           2200.,                          // dry_mass
           50.,                            // init_fuel
           30000.,                         // max_thrust
           3050.                           // exhaust velocity
    ) {
    // some values above from the Apollo lunar module
    // https://en.wikipedia.org/wiki/Apollo_Lunar_Module
}

Lander::Lander(Screen* s,
       double _x_pos,
       double _y_pos,
       double _x_vel,
       double _y_vel,
       double _orientation,
       double _spin_rate,
       double _dry_mass,
       double _init_fuel,
       double _max_thrust,
       double _exhaust_vel) {
    x_pos = _x_pos;
    y_pos = _y_pos;
    x_vel = _x_vel;
    y_vel = _y_vel;
    orientation = _orientation;
    spin_rate = _spin_rate;
    fuel = _init_fuel;
    dry_mass = _dry_mass;
    init_fuel = _init_fuel;
    thrust = _max_thrust / 2.;
    max_thrust = _max_thrust;
    exhaust_vel = _exhaust_vel;

    torque = 0.;
    // rotate about middle of thruster (at bottom of craft)
    rot_abt.x = WIDTH / 2;
    rot_abt.y = COLLISION_HEIGHT;

    if (s != NULL) {
        // bmp textures
        txtr = s->load_texture("sprites/lander.bmp");
        txtr_fire_low = s->load_texture("sprites/lander_fire_low.bmp");
        txtr_fire_med = s->load_texture("sprites/lander_fire_med.bmp");
        txtr_fire_high = s->load_texture("sprites/lander_fire_high.bmp");
        txtr_explosion = s->load_texture("sprites/explosion.bmp");
        txtr_torque_cw = s->load_texture("sprites/torque_cw.bmp");
        txtr_torque_ccw = s->load_texture("sprites/torque_ccw.bmp");

        // text textures
        fuel_txtr = s->create_text_texture("FUEL ", NULL);
        thrust_txtr = s->create_text_texture("THRUST ", NULL);
        vel_txtr = s->create_text_texture("VEL ", NULL);
        SDL_Color green = {0x33, 0xFF, 0x33, 0xFF};
        vel_txtr_green = s->create_text_texture("VEL ", &green);
    }
}

void Lander::handle(SDL_Event* e) {
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {

            case SDLK_w:
            case SDLK_UP:
                thrust += max_thrust / 10.;
                if (thrust > max_thrust) {
                    thrust = max_thrust;
                }
                break;

            case SDLK_s:
            case SDLK_DOWN:
                thrust -= max_thrust / 10.;
                if (thrust < 0.) {
                    thrust = 0.;
                }
                break;
                
            case SDLK_a:
            case SDLK_LEFT:
                torque = -MAX_TORQUE;
                break;

            case SDLK_d:
            case SDLK_RIGHT:
                torque = MAX_TORQUE;
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

            case SDLK_a:
            case SDLK_LEFT:
                torque = 0.;
                break;

            case SDLK_d:
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

void Lander::update_lines() {
    // first update the corners
    // apply a rotation matrix
    // https://en.wikipedia.org/wiki/Rotation_matrix
    double s = sin(3 * M_PI_2 + orientation);
    double c = cos(3 * M_PI_2 + orientation);
    p1x = c * sc_p1x - s * sc_p1y + WIDTH / 2 + x_pos;
    p1y = s * sc_p1x + c * sc_p1y + COLLISION_HEIGHT + y_pos;

    p2x = c * sc_p2x - s * sc_p2y + WIDTH / 2 + x_pos;
    p2y = s * sc_p2x + c * sc_p2y + COLLISION_HEIGHT + y_pos;

    p3x = c * sc_p3x - s * sc_p3y + WIDTH / 2 + x_pos;
    p3y = s * sc_p3x + c * sc_p3y + COLLISION_HEIGHT + y_pos;

    lines[0] = Line(p1x, p1y, p2x, p2y);
    lines[1] = Line(p2x, p2y, p3x, p3y);
    lines[2] = Line(p3x, p3y, p1x, p1y);
}

// publicly available next_velocity is only a prediction,
// it can't use fuel.
std::pair<double, double> Lander::next_velocity() {
    VelAccel va = next_vel_accel(false);
    std::pair<double, double> result(va.x_vel, va.y_vel);
    return result;
}

Physics::VelAccel Lander::next_vel_accel(bool real) {
    VelAccel va;
    va.x_accel = 0.;
    va.y_accel = 0.;
    if (thrusting && thrust > 0.) {
        // compute acceleration
        va.x_accel = thrust * cos(orientation) / (dry_mass + fuel);
        va.y_accel = thrust * sin(orientation) / (dry_mass + fuel);

        if (real) {
            // calculate mass change
            double dmdt = ((dry_mass + fuel) *
                          hypot(va.x_accel * DT, va.y_accel * DT) - thrust) /
                          exhaust_vel;
            fuel += dmdt * DT;

            if (fuel < 0.) {
                fuel = 0.;
                thrusting = false;
            }
        }
    }
    va.y_accel += World::g;

    va.x_vel = x_vel + va.x_accel * DT;
    va.y_vel = y_vel + va.y_accel * DT;
    return va;
}

void Lander::perturb() {
    // rotation
    spin_rate += torque * DT;
    orientation += spin_rate * DT;
    if (orientation > 2 * M_PI) {
        orientation -= 2 * M_PI;
    } else if (orientation < 0) {
        orientation += 2 * M_PI;
    }
}

void Lander::get_lines(std::vector<Line>& lines_out) {
    update_lines();
    lines_out.push_back(lines[0]);
    lines_out.push_back(lines[1]);
    lines_out.push_back(lines[2]);
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
    double vel = hypot(x_vel, y_vel) * Physics::PIXELS_PER_METER;
    if (vel <= SAFE_VEL) {
        SDL_RenderCopy(s.renderer, vel_txtr_green, NULL, &vel_text);
    } else {
        SDL_RenderCopy(s.renderer, vel_txtr, NULL, &vel_text);
    }
    SDL_RenderCopy(s.renderer, fuel_txtr, NULL, &fuel_text);

    // save old color
    Uint8 old_r, old_g, old_b, old_a;
    SDL_GetRenderDrawColor(s.renderer, &old_r, &old_g, &old_b, &old_a);
    SDL_SetRenderDrawColor(s.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // thrust bar
    SDL_RenderDrawRect(s.renderer, &thrust_bar);
    thrust_bar.w = (int) (thrust * ((double) bar_width) / max_thrust);
    SDL_RenderFillRect(s.renderer, &thrust_bar);

    // fuel bar
    SDL_RenderDrawRect(s.renderer, &fuel_bar);
    fuel_bar.w = (int) (fuel * ((double) bar_width) / init_fuel);
    SDL_RenderFillRect(s.renderer, &fuel_bar);

    // velocity bar
    // if we're in the safe zone, make bar green
    if (vel <= SAFE_VEL) {
        SDL_SetRenderDrawColor(s.renderer, 0x33, 0xFF, 0x33, 0xFF);
    }
    SDL_RenderDrawRect(s.renderer, &vel_bar);
    vel_bar.w = (int) (vel * ((double) bar_width) / MAX_VEL);
    if (vel_bar.w > bar_width) {
        vel_bar.w = bar_width; // don't go past end of bar
    }
    SDL_RenderFillRect(s.renderer, &vel_bar);

    // outline safe zone in green
    SDL_SetRenderDrawColor(s.renderer, 0x33, 0xFF, 0x33, 0xFF);
    vel_bar.w = (int) (SAFE_VEL * ((double) bar_width) / MAX_VEL);
    SDL_RenderDrawRect(s.renderer, &vel_bar);

    // add a notch to make it clearer where the safe zone ends
    SDL_RenderDrawLine(s.renderer, vel_bar.x + vel_bar.w - 1, 9,
                                   vel_bar.x + vel_bar.w - 1, 26);

    // reset old color
    SDL_SetRenderDrawColor(s.renderer, old_r, old_g, old_b, old_a);
}

void Lander::draw(Screen& s) {
    draw_status(s);

    SDL_Rect dest;
    dest.x = (int) x_pos;
    dest.y = (int) y_pos;
    dest.w = WIDTH;
    dest.h = HEIGHT;

    // choose texture based on thrust level
    SDL_Texture* t = txtr;
    if (exploded) {
        t = txtr_explosion;
    } else if (thrusting && thrust > 0.) {
        if (thrust < max_thrust / 3) {
            t = txtr_fire_low;
        } else if (thrust < 2. * max_thrust / 3.) {
            t = txtr_fire_med;
        } else {
            t = txtr_fire_high;
        }
    }

    // draw to dest, rotated by orientation
    SDL_RenderCopyEx(s.renderer,
                     t,
                     NULL,
                     &dest,
                     (M_PI_2 + orientation) * 180. / M_PI,
                     &rot_abt,
                     SDL_FLIP_NONE
    );

    // draw the cold gas thrusters, if they're on
    if (torque != 0.) {
        // the point to rotate the torque texture about
        SDL_Point rcs_rot_abt;
        rcs_rot_abt.y = rot_abt.y;

        // dimensions of the texture in pixels
        dest.h = 5;
        dest.w = 9;

        SDL_Texture* torque_txtr;
        if (torque > 0.) {
            torque_txtr = txtr_torque_cw;
            rcs_rot_abt.x = rot_abt.x;
        } else {
            torque_txtr = txtr_torque_ccw;
            dest.x += WIDTH / 2 + 2;
            rcs_rot_abt.x = rot_abt.x - (WIDTH / 2 + 2);
        }
        SDL_RenderCopyEx(s.renderer,
                         torque_txtr,
                         NULL,
                         &dest,
                         (M_PI_2 + orientation) * 180. / M_PI,
                         &rcs_rot_abt,
                         SDL_FLIP_NONE
        );
    }
}

bool Lander::is_safe_landing() {
    double vel = hypot(x_vel, y_vel) * Physics::PIXELS_PER_METER;
    thrusting = false;
    torque = 0.;
    printf("x_vel = %f, y_vel = %f\n",
            x_vel * Physics::PIXELS_PER_METER,
            y_vel * Physics::PIXELS_PER_METER);
    printf("velocity:    %f <= %f pixels per second\norientation: %f < %f radians\n",
           vel, SAFE_VEL, fabs(orientation - 3 * M_PI_2), SAFE_ORIENTATION);
    return vel <= SAFE_VEL && fabs(orientation - 3 * M_PI_2) < SAFE_ORIENTATION;
}

void Lander::explode() {
    exploded = true;
}

double Lander::fuel_remaining() {
    return fuel / init_fuel;
}

Lander::~Lander() {
    if (txtr != NULL) {
        SDL_DestroyTexture(txtr);
        SDL_DestroyTexture(txtr_fire_low);
        SDL_DestroyTexture(txtr_fire_med);
        SDL_DestroyTexture(txtr_fire_high);
        SDL_DestroyTexture(txtr_explosion);
        SDL_DestroyTexture(txtr_torque_cw);
        SDL_DestroyTexture(txtr_torque_ccw);
        SDL_DestroyTexture(fuel_txtr);
        SDL_DestroyTexture(thrust_txtr);
        SDL_DestroyTexture(vel_txtr);
        SDL_DestroyTexture(vel_txtr_green);
    }
}
