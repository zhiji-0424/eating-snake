/*
 *  Copyright (c) 2022-2025 zhiji_0424
 *  eating-snake is licensed under Mulan PSL v2.
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 
 *  You may obtain a copy of Mulan PSL v2 at:
 *           http://license.coscl.org.cn/MulanPSL2 
 *  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 *  See the Mulan PSL v2 for more details.  
 */

#include "main.h"


const char * const title = "====eating-snake====";

size_t time1, time2;
static inline void get_time(size_t *rs);

int ofs_x = 1, ofs_y = 4;
void draw_box();
void draw_food(food_t *food);
void draw_snake(snake_t *snake);


int main()
{
    initscr();
    cbreak();
    noecho();
    timeout(0);
    
    snake_env_t *env = snake_env_create();
    score_read(env);
    if (env == null) return -1;
    signed char ch_direc = ' ';
    
    while (true)
    {
        get_time(&time2);
        if (time2 - time1 >= 200)
        {
            time1 = time2;

            ch_direc = getch();
            snake_change_direc(env, ch_direc);
            snake_forward(env);
            if (is_snake_ate_food(env))
            {
                score_add(env, 1);
                score_update(env);
                food_reappear(env);
            }
            else
            {
                snake_del_tail(env);
            }
            if (is_snake_ate_self(env))
            {
                score_write(env);
                env->score->score = 0;
                while (get_snake(env)->len != 1)
                {
                    snake_del_tail(env);
                }
            }
            
            /* Start drawing */
            clear();
            draw_food(get_food(env));
            draw_snake(get_snake(env));
            draw_box(env);

            refresh();
        }
    }
    
    snake_env_destroy(env);
    
    endwin();
    return 0;
}

static inline void get_time(size_t *rs)
{
    struct timeval tv;
    gettimeofday(&tv, null);
    *rs = tv.tv_sec*1000 + tv.tv_usec/1000;
}

void draw_box(snake_env_t *env)
{
    mvaddstr(0, 0, title);
    mvprintw(1, 0, "Score: %d", get_score(env)->score);
    if (env->score->score < env->score->highest)
    {
        mvprintw(2, 0, "Highest: %d",
            get_score(env)->highest);
    }

    /* This box must bigger than stage. */
    for (int x = 0; x < stage_w+2; ++x)
    {
        mvaddch(ofs_y, x*2, '#');
        mvaddch(stage_h+1+ofs_y, x*2, '#');
    }
    for (int y = 0+ofs_y; y < stage_h+2+ofs_y; ++y)
    {
        mvaddch(y, 0, '#');
        mvaddch(y, (stage_w+1)*2, '#');
    }
}

void draw_food(food_t *food)
{
    mvaddch(food->y+ofs_y+1,
        (food->x+ofs_x)*2, food->style);
}

void draw_snake(snake_t *snake)
{
    body_t *node = snake->node;
    while (node != null)
    {
        mvaddch(node->y+ofs_y+1,
            (node->x+ofs_x)*2, node->style);
        node = node->next;
    }
}


