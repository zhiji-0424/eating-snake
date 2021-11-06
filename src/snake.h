/* Copyright (c) [2021] [tyri_ji]
 * [eating-snake] is licensed under Mulan PSL v2.
 * You can use this software according to the terms and co>
 * You may obtain a copy of Mulan PSL v2 at:
 *         http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT >
 * See the Mulan PSL v2 for more details.
 */

#ifndef SNAKE_H
#define SNAKE_H


#include <stdlib.h>
#include "locals.h"


void snake_create();
void snake_destroy();

snake_t *get_snake_head();
int get_snake_len();
int get_snake_direc();

/* Won't delete tail node after forwarding. */
void snake_forward();
void snake_del_tail();
/* It is smart. */
void snake_change_direc(int direc);


int is_snake_ate_food(snake_t *head, point_t food);
int is_snake_ate_self(snake_t *head);


#endif
