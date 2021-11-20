/* Copyright (c) [2021] [tyri_ji]
 * [eating-snake] is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2. 
 * You may obtain a copy of Mulan PSL v2 at:
 *        http://license.coscl.org.cn/MulanPSL2 
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 * See the Mulan PSL v2 for more details. * See the Mulan PSL v2 for more details.
 */

#include "food.h"


food_t food = { 0, 0, food_style };


static int is_in_snake(int x, int y, snake_t *head)
{
	snake_t *t1 = head;
	while (t1 != null)
	{
		if ((x == t1->x)
		 && (y == t1->y))
		{
			return 1;
		}
		t1 = t1->next;
	}
	return 0;
}

void food_reappear(snake_t *head)
{
	do
	{
		food.x = rand() % stage_w;
		food.y = rand() % stage_h;
	}
	while (is_in_snake(food.x, food.y, head));
}

point_t get_foodxy()
{
	point_t xy = { food.x, food.y };
	return xy;
}

food_t get_food()
{
	return food;
}
