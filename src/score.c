/*
 *  Copyright (c) 2022-2025 zhiji_0424
 *  eating-snake is licensed under Mulan PSL v2.
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 
 *  You may obtain a copy of Mulan PSL v2 at:
 *           http://license.coscl.org.cn/MulanPSL2 
 *  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 *  See the Mulan PSL v2 for more details.  
 */

#include "score.h"


int score   = 0;
int highest = 0;

int get_score()
{
	return score;
}

void set_score(int n)
{
	score = n;
}

int get_highest()
{
	return highest;
}

void set_highest(int n)
{
	highest = n;
}

void score_rmall()
{
	score   = 0;
	highest = 0;
	score_write();
}

void score_add(int n)
{
	score += n;
}

void score_update()
{
	if (score > highest)
	{
		highest = score;
	}
}

void score_read()
{
	FILE* fp = fopen(score_file, "rb");
	if (fp != null)
	{
		fread(&highest, sizeof(highest), 1, fp);
		fclose(fp);
		return;
	}
	highest = 0;
}

void score_write()
{
	FILE* fp = fopen(score_file, "wb");
	if (fp != null)
	{
		fwrite(&highest, sizeof(highest), 1, fp);
		fclose(fp);
		return;
	}
}


