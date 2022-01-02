/*
 *  Copyright (c) 2022-2025 zhiji_0424
 *  eating-snake is licensed under Mulan PSL v2.
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 
 *  You may obtain a copy of Mulan PSL v2 at:
 *           http://license.coscl.org.cn/MulanPSL2 
 *  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 *  See the Mulan PSL v2 for more details.  
 */

#include "food.h"


static inline int is_in_snake(int x, int y, snake_t *snake)
{
    /* check_ret(snake, false); */
    /* check_ret(snake->node, false); */
    body_t *t1 = snake->node;
    while (t1 != null)
    {
        if ((x == t1->x)
         && (y == t1->y))
        {
            return true;
        }
        t1 = t1->next;
    }
    return false;
}

void food_reappear(snake_env_t *env)
{
    check(env);
    check(env->food);
    check(env->snake);
    check(env->snake->node);
    do
    {
        env->food->x = rand() % stage_w;
        env->food->y = rand() % stage_h;
    }
    while (is_in_snake(
        env->food->x, env->food->y, env->snake));
}

