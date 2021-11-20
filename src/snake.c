/* Copyright (c) [2021] [tyri_ji]
 * [eating-snake] is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2. 
 * You may obtain a copy of Mulan PSL v2 at:
 *        http://license.coscl.org.cn/MulanPSL2 
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 * See the Mulan PSL v2 for more details. * See the Mulan PSL v2 for more details.
 */

#include "snake.h"


snake_t *snake_head = null;
int snake_len   = 0;
int snake_direc = 0;


void snake_create()
{
	snake_head = (snake_t *)
			malloc(sizeof(snake_t));
	snake_head->x = stage_w/2;
	snake_head->y = stage_h/2;
	snake_head->style = head_style;
	snake_direc = to_up;
	snake_head->next = null;
	snake_len = 1;
}

void snake_destroy()
{
	snake_t *t1 = snake_head;
	snake_t *t2 = t1;

	// It will be a head with no body.
	while (t1->next != null)
	{
		t2 = t1->next;
		t1->next = t1->next->next;
		free(t2);
		t2 = null;
	}
	// Now let's delete the head.
	free(snake_head);
	snake_head = null;
	snake_len  = 0;
}


snake_t *get_snake_head()
{
	return snake_head;
}

int get_snake_len()
{
	return snake_len;
}

int get_snake_direc()
{
	return snake_direc;
}


// Update Snake's head
static void update_head(snake_t *head,
                        int direc)
{	
	switch (direc)
	{
	case to_up:
		head->y -= 1;
		break;
	case to_down:
		head->y += 1;
		break;
	case to_left:
		head->x -= 1;
		break;
	case to_right:
		head->x += 1;
		break;
	}
	
	// Walk throught Wall
	if (head->x < 0)
	{
		head->x = stage_w-1;
	}
	else if (head->x > stage_w-1)
	{
		head->x = 0;
	}
	else if (head->y < 0)
	{
		head->y = stage_h-1;
	}
	else if (head->y > stage_h-1)
	{
		head->y = 0;
	}
}

void snake_forward()
{
	snake_t *t1 = (snake_t *)
	              malloc(sizeof(snake_t));
	t1->next = snake_head->next;
	snake_head->next = t1;
	snake_len += 1;
	
	t1->x = snake_head->x;
	t1->y = snake_head->y;
	if (snake_len > 2)
	{
		// t1 is a body node.
		t1->style = body_style;
	}
	else
	{
		// t1 is the tail.
		t1->style = tail_style;
	}
	
	update_head(snake_head, snake_direc);
}

void snake_del_tail()
{
	if (snake_len > 1)
	{
		snake_t *t1 = snake_head;
		snake_t *t2 = t1;
		if (t1 == null) { return; }
		while (t1->next != null)
		{
			t2 = t1;
			t1 = t1->next;
		}
		t2->next = null;
		free(t1);
		snake_len -= 1;
		
		if (snake_len > 1)
		{
			// t2 is the tail now.
			t2->style = tail_style;
		}
	}
}

void snake_change_direc(int direc)
{
	if ((((snake_direc == to_up)
	   || (snake_direc == to_down))
	  && ((direc == to_left)
	   || (direc == to_right)))
	 || (((snake_direc == to_left)
	   || (snake_direc == to_right))
	  && ((direc == to_up)
	   || (direc == to_down))))
	{
		snake_direc = direc;
	}
}


int is_snake_ate_food(snake_t *head, point_t food)
{
	if ((head->x == food.x)
	 && (head->y == food.y))
	{
		return 1;
	}
	return 0;
}

int is_snake_ate_self(snake_t *head)
{
	if (head == null) { return 0; }
	snake_t *t1 = head->next;
	while (t1 != null)
	{
		if ((t1->x == head->x)
		 && (t1->y == head->y))
		{
			return 1;
		}
		t1 = t1->next;
	}
	return 0;
}
