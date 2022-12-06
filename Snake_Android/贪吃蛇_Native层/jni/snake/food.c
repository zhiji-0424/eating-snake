#include "food.h"

static food_t food = { 0, 0 };

food_t get_food() {
    return food;
}

void create_food(int32_t w, int32_t h) {
    food.x = rand() % w;
    food.y = rand() % h;
}