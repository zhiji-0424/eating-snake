/* Copyright (c) [2021] [tyri_ji]
 * [eating-snake] is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2. 
 * You may obtain a copy of Mulan PSL v2 at:
 *        http://license.coscl.org.cn/MulanPSL2 
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 * See the Mulan PSL v2 for more details. * See the Mulan PSL v2 for more details.
 */

#ifndef SCORE_H
#define SCORE_H


#include <stdio.h>
#include "locals.h"


int  get_score();
void set_score(int n);

int  get_highest();
void set_highest(int n);

// Make the value of all to 0,
// include that in the file system.
void score_rmall();

// score += n;
void score_add(int n);

// If 'score' > 'highest', 'highest' = 'score';
void score_update();

// Read score from file system
void score_read();

// Write the highest score to file system
void score_write();


#endif
