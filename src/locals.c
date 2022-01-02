/*
 *  Copyright (c) 2022-2025 zhiji_0424
 *  eating-snake is licensed under Mulan PSL v2.
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 
 *  You may obtain a copy of Mulan PSL v2 at:
 *           http://license.coscl.org.cn/MulanPSL2 
 *  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 *  See the Mulan PSL v2 for more details.  
 */

#include "locals.h"


/* Create or Destroy snake ============================== */
snake_t *snake_create()
{
    snake_t *snake =
        malloc(sizeof(snake_t));
    check_ret(snake, null);    /**/
    snake->direc = to_up;
    snake->len = 1;

    snake->node =
        malloc(sizeof(body_t));
    check_ret(snake->node, null);    /**/
    snake->node->x = stage_w/2;
    snake->node->y = stage_h/2;
    snake->node->next  = null;
    snake->node->style = head_style;
    return snake;
}

void snake_destroy(snake_t *snake)
{
    check(snake);
    check(snake->node);
    body_t *t1 = snake->node;
    body_t *t2 = t1;

    /* It will be a head node with no body node. */
    while (t1->next != null)
    {
        t2 = t1->next;
        t1->next = t1->next->next;
        free(t2);
        t2 = null;
    }
    /* Now let's delete the head node. */
    free(snake->node);
    free(snake);
}


/* Create or Destroy food =================================== */
food_t *food_create()
{
    food_t *food =
        malloc(sizeof(food_t));
    check_ret(food, null);    /**/
    food->x = 0;
    food->y = 0;
    food->style = food_style;
    return food;
}

void food_destroy(food_t *food)
{
    check(food);    /**/
    free(food);
}


/* Create or Destroy score =================================== */
score_t *score_create()
{
    score_t *score =
        malloc(sizeof(score_t));
    check_ret(score, null);    /**/
    score->score   = 0;
    score->highest = 0;
    return score;
}

void score_destroy(score_t *score)
{
    check(score);    /**/
    free(score);
}


/* Create or Destroy environment ============================== */
snake_env_t *snake_env_create()
{
    snake_env_t *env =
        malloc(sizeof(snake_env_t));
    check_ret(env, null);    /**/
    env->snake = snake_create();
    env->food  = food_create();
    env->score = score_create();
    if ((env->snake == null)
     || (env->food == null)
     || (env->score == null))
    {
printf("--ERR: Not Enough Memory(N-E-RAM)\n");
        snake_env_destroy(env);
        return null;
    }
    return env;
}

void snake_env_destroy(snake_env_t *env)
{
    snake_destroy(env->snake);
    food_destroy(env->food);
    score_destroy(env->score);
    check(env);    /**/
    free(env);
}

