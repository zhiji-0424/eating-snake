#ifndef SNAKE_H
#define SNAKE_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include "food.h"

#define up    0
#define down  1
#define left  2
#define right 3

typedef struct node_st {
    int32_t x, y;
    int32_t length;
    int32_t direction;
    struct node_st* next;
} node_t, snake_t;

void snake_forward(int32_t stage_w, int32_t stage_h);  // 不会删除尾巴
void snake_del_tail(void);
void snake_change_direc(int32_t direc);

int snake_ate_food(void);
int snake_ate_self(void);
int food_in_snake(void);

snake_t* get_snake(void);


#ifdef __cplusplus
}
#endif
#endif