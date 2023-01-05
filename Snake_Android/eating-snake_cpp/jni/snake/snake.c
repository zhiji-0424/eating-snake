#include "snake.h"
#include "tools/Log.h"

#define LOGE(...) Log_E("snake", __VA_ARGS__)
#if !defined(NDEBUG) && !defined(main_NDEBUG)
#  define LOGD(...) Log_D("snake", __VA_ARGS__);
#else
#  define LOGD(...) (void)0
#endif

static snake_t snake = { 0, 0, 1, up, NULL };

static node_t* malloc_node()
{
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if (!node)
        LOGE("malloc_node: out of memory!");
    memset(node, 0, sizeof(node_t));
    return node;
}

static void update_head(int32_t stage_w, int32_t stage_h)
{
    switch (snake.direction) {
        case up:
            snake.y++;
            break;
        case down:
            snake.y--;
            break;
        case left:
            snake.x--;
            break;
        case right:
            snake.x++;
            break;
    }
    
    /* Walk throught Wall */
    if (snake.x < 0) {
        snake.x = stage_w-1;
    }
    else if (snake.x > stage_w-1) {
        snake.x = 0;
    }
    else if (snake.y < 0) {
        snake.y = stage_h-1;
    }
    else if (snake.y > stage_h-1) {
        snake.y = 0;
    }
}

void snake_forward(int32_t stage_w, int32_t stage_h)
{
    node_t* node = malloc_node();
    node->next = snake.next;
    snake.next = node;
    snake.length += 1;

    node->x = snake.x;
    node->y = snake.y;

    update_head(stage_w, stage_h);
}

void snake_del_tail()
{
    if (snake.length > 1) {
        node_t* t1 = &snake;
        node_t* t2 = t1;
        while (t1->next != NULL) {
            t2 = t1;
            t1 = t1->next;
        }
        free(t1);
        t2->next = NULL;
        snake.length--;
    }
}

void snake_change_direc(int32_t direction)
{
    if ( ((snake.direction == up || snake.direction == down) && (direction == left || direction == right))
      || ((snake.direction == left || snake.direction == right) && (direction == up || direction == down)) )
    {
        snake.direction = direction;
    }
}

int snake_ate_food()
{
    food_t food = get_food();
    if (snake.x == food.x && snake.y == food.y)
        return 1;
    return 0;
}

int snake_ate_self()
{
    node_t* node = snake.next;
    while (node) {
        if (node->x == snake.x && node->y == snake.y)
            return 1;
        node = node->next;
    }
    return 0;
}

int food_in_snake(void)
{
    food_t food = get_food();
    node_t* node = &snake;
    while (node) {
        if (node->x == food.x && node->y == food.y)
            return 1;
        node = node->next;
    }
    return 0;
}

snake_t* get_snake() {
    return &snake;
}

