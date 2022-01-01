/*
 *  Copyright (c) 2022-2025 zhiji_0424
 *  eating-snake is licensed under Mulan PSL v2.
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 
 *  You may obtain a copy of Mulan PSL v2 at:
 *           http://license.coscl.org.cn/MulanPSL2 
 *  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 *  See the Mulan PSL v2 for more details.  
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

food_t get_food();


#endif
