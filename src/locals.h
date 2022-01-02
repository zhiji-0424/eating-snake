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


#define stage_w 20
#define stage_h 20

// Direction
#define to_up    '2'
#define to_down  '8'
#define to_left  '4'
#define to_right '6'

#define CLEAR "\33[H\33[2J"     //\33[3J"
#define score_file "./game-score"

#define food_style '@'
#define head_style 'O'
#define body_style 'o'
#define tail_style '0'

// The Information of Point
typedef struct
{
    int x;
    int y;
} point_t;

// The Infomation of Food
typedef struct
{
    int x;
    int y;
    char style;
} food_t;

// The Infomation of Snake
struct node_st;
typedef struct node_st snake_t;
struct node_st {
    int x;
    int y;
    char style;
    snake_t *next;
};

#ifndef null
#ifdef __cplusplus
#define null 0
#else
#define null ((void *)0)
#endif  /* endif __cpp */
#endif  /* endif null */

#ifndef false
#define false 0
#endif
#ifndef true
#define true (!false)
#endif


#endif
