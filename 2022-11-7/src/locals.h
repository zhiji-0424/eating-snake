/*
 *  Copyright (c) 2022-2025 zhiji_0424
 *  eating-snake is licensed under Mulan PSL v2.
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 
 *  You may obtain a copy of Mulan PSL v2 at:
 *           http://license.coscl.org.cn/MulanPSL2 
 *  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 *  See the Mulan PSL v2 for more details.  
 */

#ifndef LOCALS_H
#define LOCALS_H


#include <stdio.h>
#include <stdlib.h>

#define stage_w 20
#define stage_h 20

/* Direction */
#define to_up    '2'
#define to_down  '8'
#define to_left  '4'
#define to_right '6'

#define CLEAR "\33[H\33[2J"
#define score_file "./game-score"

#define food_style '@'
#define head_style 'O'
#define body_style 'O' // 'o'
#define tail_style 'O' // '0'

/* The Information of Point */
typedef struct
{
    int x;
    int y;
} point_t;

/* The Infomation of Food */
typedef struct
{
    int x;
    int y;
    unsigned char style;
} food_t;

/* The Infomation of Snake body */
struct node_st;
typedef struct node_st body_t;
struct node_st {
    body_t *next;
    int x;
    int y;
    unsigned char style;
};

/* The Information of Snake head */
typedef struct
{
    body_t *node;
    unsigned char len;
    unsigned char direc;
} snake_t;

/* Two score */
typedef struct
{
    unsigned int score;
    unsigned int highest;
} score_t;

/* Snake data environment !!! */
typedef struct
{
    snake_t *snake;
    food_t  *food;
    score_t *score;
} snake_env_t;


/* Create or Destroy snake */
snake_t *snake_create();
void snake_destroy(snake_t *snake_head);
/* Create or Destroy food */
food_t *food_create();
void food_destroy(food_t *food);
/* Create or Destroy score */
score_t *score_create();
void score_destroy(score_t *score);
/* Create or Destroy environment */
snake_env_t *snake_env_create();
void snake_env_destroy(snake_env_t *env);


#ifndef null
#  ifdef __cplusplus
#    define null 0
#  else
#    define null (void *)0
#  endif
#endif

#ifndef false
#  define false 0
#endif
#ifndef true
#  define true !false
#endif

/* In order to make sure that operation is not null */
/* Check the ptr. Only if ptf==null, return retv. */
#define check_ret(ptr, retv) if (ptr == null) return retv
#define check(ptr) if (ptr == null) return


#endif
