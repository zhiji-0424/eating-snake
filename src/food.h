/* Copyright (c) [2021] [tyri_ji]
 * [eating-snake] is licensed under Mulan PSL v2.
 * You can use this software according to the terms and co>
 * You may obtain a copy of Mulan PSL v2 at:
 *         http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT >
 * See the Mulan PSL v2 for more details.
 */

#ifndef FOOD_H
#define FOOD_H


#include <stdlib.h>
#include <time.h>
#include "locals.h"


/* Recalculate the Position of Food */
void food_reappear(snake_t *head);

/* Get the Position of Food. */
point_t get_foodxy();


#endif
