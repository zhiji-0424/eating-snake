/*
 *  Copyright (c) 2022-2025 zhiji_0424
 *  eating-snake is licensed under Mulan PSL v2.
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 
 *  You may obtain a copy of Mulan PSL v2 at:
 *           http://license.coscl.org.cn/MulanPSL2 
 *  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 *  See the Mulan PSL v2 for more details.  
 */

#ifndef SCORE_H
#define SCORE_H


#include <stdio.h>
#include "locals.h"


void set_score(snake_env_t *env, unsigned int n);
void set_highest(snake_env_t *env, unsigned int n);
/* Make the value of all to 0,
 * include that in the file system. */
void score_rmall(snake_env_t *env);
/* score += n; */
void score_add(snake_env_t *env, unsigned int n);
/* If 'score' > 'highest', 'highest' = 'score'; */
void score_update(snake_env_t *env);
/* Read score from file system */
void score_read(snake_env_t *env);
/* Write the highest score to file system */
void score_write(snake_env_t *env);


#endif
