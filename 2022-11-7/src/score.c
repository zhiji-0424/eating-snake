/*
 *  Copyright (c) 2022-2025 zhiji_0424
 *  eating-snake is licensed under Mulan PSL v2.
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 
 *  You may obtain a copy of Mulan PSL v2 at:
 *           http://license.coscl.org.cn/MulanPSL2 
 *  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 *  See the Mulan PSL v2 for more details.  
 */

#include "score.h"


void set_score(snake_env_t *env, unsigned int n)
{
    check(env);
    check(env->score);
    env->score->score = n;
}

void set_highest(snake_env_t *env, unsigned int n)
{
    check(env);
    check(env->score);
    env->score->highest = n;
}

void score_rmall(snake_env_t *env)
{
    check(env);
    check(env->score);
    env->score->score   = 0;
    env->score->highest = 0;
    score_write(env);
}

void score_add(snake_env_t *env, unsigned int n)
{
    check(env);
    check(env->score);
    env->score->score += n;
}

void score_update(snake_env_t *env)
{
    check(env);
    check(env->score);
    if (env->score->score
      > env->score->highest)
    {
        env->score->highest
        = env->score->score;
    }
}

void score_read(snake_env_t *env)
{
    check(env);
    check(env->score);
    FILE* fp = fopen(score_file, "rb");
    if (fp != null)
    {
        if (0 == fread( &(env->score->highest),
               sizeof(env->score->highest), 1, fp))
            env->score->highest = 0;
        fclose(fp);
        return;
    }
    env->score->highest = 0;
}

void score_write(snake_env_t *env)
{
    check(env);
    check(env->score);
    FILE* fp = fopen(score_file, "wb");
    if (fp != null)
    {
        fwrite( &(env->score->highest),
                sizeof(env->score->highest), 1, fp);
        fclose(fp);
        return;
    }
}

score_t *get_score(snake_env_t *env)
{
    return env->score;
}

