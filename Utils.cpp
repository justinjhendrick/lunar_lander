#include <random>
#include "Utils.hpp"

std::default_random_engine Utils::rand_generator;

void Utils::init(unsigned int seed) {
    rand_generator.seed(seed);
}

SDL_Texture* Utils::load_texture(
        SDL_Renderer* r,
        const char filename[],
        bool has_alpha) {
    SDL_Surface* surf = SDL_LoadBMP(filename);
    if (surf == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture* result = SDL_CreateTextureFromSurface(r, surf);
    if (result == NULL) {
        fprintf(stderr, "no convert to texture: %s\n", SDL_GetError());
    }
    SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surf);
    return result;
}

SDL_Texture* Utils::create_text_texture(Screen& s,
                                        const char text[],
                                        SDL_Color* color) {
    SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
    if (color == NULL) {
        color = &white;
    }
    SDL_Surface* surf = TTF_RenderText_Solid(s.font, text, *color);
    if (surf == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture* result = SDL_CreateTextureFromSurface(s.renderer, surf);
    if (result == NULL) {
        fprintf(stderr, "no convert to texture: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surf);
    return result;
}

float Utils::rand_float(float lower_bd, float upper_bd) {
    std::uniform_real_distribution<float> dist(lower_bd, upper_bd);
    return dist(rand_generator);
}

float Utils::rand_int(int lower_bd, int upper_bd) {
    std::uniform_int_distribution<int> dist(lower_bd, upper_bd);
    return dist(rand_generator);
}

// puts difference in 'out'
// returns true if it'll pass through zero
bool Utils::angle_diff(float a, float b, float* out) {
    float diff = fabs(a - b);
    bool result = false;
    // shouldn't ever rotate more than half circle
    if (diff > M_PI) {
        diff = 2 * M_PI - diff;
        result = true;
    }
    *out = diff;
    return result;
}

int Utils::round_nearest_int(float a) {
    return (int) floor(a + 0.5);
}
