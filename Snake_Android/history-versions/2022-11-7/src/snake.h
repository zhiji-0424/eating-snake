/*
 *  Copyright (c) 2022-2025 zhiji_0424
 *  eating-snake is licensed under Mulan PSL v2.
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 
 *  You may obtain a copy of Mulan PSL v2 at:
 *           http://license.coscl.org.cn/MulanPSL2 
 *  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 *  See the Mulan PSL v2 for more details.  
 */

#ifndef SNAKE_H
#define SNAKE_H


#include <stdlib.h>
#include "locals.h"


/* Won't delete tail node after forwarding. */
void snake_forward(snake_env_t *env);
void snake_del_tail(snake_env_t *env);
/* It is smart. */
void snake_change_direc(snake_env_t *env, unsigned char direc);

int is_snake_ate_food(snake_env_t *env);
int is_snake_ate_self(snake_env_t *env);

snake_t *get_snake(snake_env_t *env);


#endif
