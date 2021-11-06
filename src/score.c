/* Copyright (c) [2021] [tyri_ji]
 * [eating-snake] is licensed under Mulan PSL v2.
 * You can use this software according to the terms and co>
 * You may obtain a copy of Mulan PSL v2 at:
 *         http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT >
 * See the Mulan PSL v2 for more details.
 */

#include "score.h"


int score;
int highest;

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
	FILE* fp = fopen(score_file, "r");
	if (fp != NULL)
	{
		fread(&highest, sizeof(highest), 1, fp);
		fclose(fp);
		return;
	}
	highest = 0;
}

void score_write()
{
	FILE* fp = fopen(score_file, "w");
	if (fp != NULL)
	{
		fwrite(&highest, sizeof(highest), 1, fp);
		fclose(fp);
		score = 0;
		return;
	}
}


