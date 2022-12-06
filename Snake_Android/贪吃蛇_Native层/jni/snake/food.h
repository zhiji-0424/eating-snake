#ifndef FOOD_H
#define FOOD_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int32_t x, y;
} food_t;

void create_food(int32_t w, int32_t h);
food_t get_food(void);


#ifdef __cplusplus
}
#endif
#endif