/*
 *  Copyright (c) 2022-2025 zhiji_0424
 *  eating-snake is licensed under Mulan PSL v2.
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 
 *  You may obtain a copy of Mulan PSL v2 at:
 *           http://license.coscl.org.cn/MulanPSL2 
 *  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 *  See the Mulan PSL v2 for more details.  
 */

#include "snake.h"


/* Update Snake's head */
static inline void update_head(snake_t *snake, unsigned char direc)
{
    /* check(snake); */
    /* check(snake->node); */
    body_t *node = snake->node;
    switch (direc)
    {
    case to_up:
        --node->y;
        break;
    case to_down:
        ++node->y;
        break;
    case to_left:
        --node->x;
        break;
    case to_right:
        ++node->x;
        break;
    }
    
    /* Walk throught Wall */
    if (node->x < 0)
    {
        node->x = stage_w-1;
    }
    else if (node->x > stage_w-1)
    {
        node->x = 0;
    }
    else if (node->y < 0)
    {
        node->y = stage_h-1;
    }
    else if (node->y > stage_h-1)
    {
        node->y = 0;
    }
}

void snake_forward(snake_env_t *env)
{
    check(env);
    check(env->snake);
    check(env->snake->node);
    body_t *node = env->snake->node;
    body_t *t1 =
        malloc(sizeof(body_t));
    t1->next = node->next;
    node->next = t1;
    ++env->snake->len;
    
    t1->x = node->x;
    t1->y = node->y;
    if (env->snake->len > 2)
    {
        /* t1 is a body node. */
        t1->style = body_style;
    }
    else
    {
        /* t1 is the tail. */
        t1->style = tail_style;
    }
    
    update_head(env->snake, env->snake->direc);
}

void snake_del_tail(snake_env_t *env)
{
    check(env);
    check(env->snake);
    check(env->snake->node);
    if (env->snake->len > 1)
    {
        body_t *t1 = env->snake->node;
        body_t *t2 = t1;
        if (t1 == null) { return; }
        while (t1->next != null)
        {
            t2 = t1;
            t1 = t1->next;
        }
        t2->next = null;
        free(t1);
        --env->snake->len;
        
        if (env->snake->len > 1)
        {
            /* If t2 is the tail now. */
            t2->style = tail_style;
        }
    }
}

void snake_change_direc(snake_env_t *env, unsigned char direc)
{
    check(env);
    check(env->snake);
    check(env->snake->node);
    if ((((env->snake->direc == to_up)
       || (env->snake->direc == to_down))
      && ((direc == to_left)
       || (direc == to_right)))
     || (((env->snake->direc == to_left)
       || (env->snake->direc == to_right))
      && ((direc == to_up)
       || (direc == to_down))))
    {
        env->snake->direc = direc;
    }
}


int is_snake_ate_food(snake_env_t *env)
{
    check_ret(env, false);
    check_ret(env->snake, false);
    check_ret(env->snake->node, false);
    check_ret(env->food, false);
    if ((env->snake->node->x == env->food->x)
     && (env->snake->node->y == env->food->y))
    {
        return true;
    }
    return false;
}

int is_snake_ate_self(snake_env_t *env)
{
    check_ret(env, false);
    check_ret(env->snake, false);
    check_ret(env->snake->node, false);
    body_t *t1 = env->snake->node->next;
    while (t1 != null)
    {
        if ((t1->x == env->snake->node->x)
         && (t1->y == env->snake->node->y))
        {
            return true;
        }
        t1 = t1->next;
    }
    return false;
}
