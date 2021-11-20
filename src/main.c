/* Copyright (c) [2021] [tyri_ji]
 * [eating-snake] is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2. 
 * You may obtain a copy of Mulan PSL v2 at:
 *        http://license.coscl.org.cn/MulanPSL2 
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 * See the Mulan PSL v2 for more details. * See the Mulan PSL v2 for more details.
 */

#include "main.h"

#include <termio.h>
#include <stdio.h>
char getin()
{
	char in;
	struct termios new_settings;
	struct termios stored_settings;
	tcgetattr(0, &stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_cc[VTIME] = 0;
	tcgetattr(0, &stored_settings);
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &new_settings);
	in = getchar();
	tcsetattr(0, TCSANOW, &stored_settings);
	return in;
}

#define main_acti 0
#define game_acti 1

#define title "##%##贪吃蛇##%##"
#define print(format, ...) printf(format, ##__VA_ARGS__);printf("\n")	//目的是添加换行
#define while_run(func, n) for(int lli=0;lli<n;lli+=1){func;}			//重复执行N次


//毫秒级时间戳
struct timeval tv;
long int time1 = 0;
long int time2 = 0;
//lltv和lla是地址
#define get_mtime(lltv, lla) \
gettimeofday(lltv, null);\
*(lla) = (*(lltv)).tv_sec*1000 + (*(lltv)).tv_usec/1000


bool main_running = true;
bool game_running = false;
bool is_died = false;
bool is_won  = false;
Score *score = null;
Food  *food  = null;
Snake *snake = null;


void break_main();
void break_game();

void key_event(int);
void game();
void helper();
void setting();


//针对键盘输入的线程
char ch = '\0';
pthread_t llin_id;
void *llin(void *arg)
{
	// arg = (char *)arg;
	while (game_running)
	{
		*((char *)arg) = getin();
		key_event(game_acti);
	}
	return null;
}
//id是地址(指针)
#define start_thread(id, func, arg) pthread_create(id, null, func, (void *)arg)
#define exit_thread(id) pthread_cancel(id)


void init(int index)
{
	switch (index)
	{
	case main_acti:
		score = new Score();
		break;
	
	case game_acti:
		game_running = true;
		start_thread(&llin_id, llin, &ch);
		snake = new Snake();
		food  = new Food();
		food->create(snake->get_head());
		break;
	}
}
void destroy(int index)
{
	switch (index)
	{
	case main_acti:
		printf(CLEAR);
		delete(score);
		score = null;
		break;
	
	case game_acti:
		exit_thread(llin_id);
		score->write();
		break_game();
		delete(food);
		food = null;
		delete(snake);
		snake = null;
		
		if (is_won)
		{
			printf("\n 你通关啦！\n");
			printf("\t[确定]\n\t");
			ch = getin();
			is_won = false;
		}
		else if (is_died)
		{
			printf("\n 你失败了！\n");
			printf("  撞到了自己\n");
			printf("[回主界面]");
			ch = getin();
			is_died = false;
		}
		printf(CLEAR);
		break;
	}
}

void show_title()
{
	if (game_running)
	{
		while_run(printf("="), 16);
		printf("%s", title);
		while_run(printf("="), 16);
		printf("\n");
	}
	else
	{
		print("%s", title);
	}
}
void show_score()
{
	if (game_running)
	{
		print(" 分数: %d", score->score);
		if (score->score < score->highest)
		{
			print(" 目标: %d", score->highest);
		}
	}
	else
	{
		print(" 分数: %d", score->highest);
	}
}
void show_tip(int index)
{
	switch (index)
	{
	case main_acti:
		while_run(printf("\n"), 3);
		print(" 按空格开始游戏");
		print(" 按H打开帮助界面");
		print(" 按S打开设置界面");
		break;
	case game_acti:
		print(" ====WSAD:2846:↑↓←→");
		print(" ====Q键退出");
		break;
	}
}

void helper(void)
{
	while (true)
	{
		printf(CLEAR);
		show_title();
		print(" 帮助\n");
		
		print(" 使用键盘即可操作。");
		print(" 撞到自己时就失败，");
		print(" 身体占满全部就通关。");
		while_run(printf("\n"), 2);
		
		print(" 请将BUG反馈给");
		print(" \t邮箱[zhiji_0424@qq.com]");
		print(" 谢谢使用。");
		printf("\n");
		print(" 设计: [@欢]");
		print(" 制作: [ll]");
		
		ch = getin();
		if ((ch =='Q')
			|| (ch == ' ')
			|| (ch == '\n'))
		{
			break;
		}
	}
}

void setting()
{
	bool is_rmed_score = false;
	
	while (true)
	{
		printf(CLEAR);
		show_title();
		print(" 设置\n");
		
		print(" 请按选项前的键:");
		printf("  C. 清零分数[!]");
		if (is_rmed_score) { print(" (完成)"); }
		else { printf("\n"); }
		
		ch = getin();
		if ((ch =='Q')
			|| (ch == ' ')
			|| (ch == '\n'))
		{
			break;
		}
		else if ((ch == 'C')
			&& (!is_rmed_score))
		{
			print("\n\t[危险]再按一次!!");
			ch = getin();
			if (ch == 'C')
			{
				score->rmall();
				is_rmed_score = true;
				continue;
			}
			print("\t操作取消。");
			sleep(1);
		}
	}
}

void break_main()
{
	main_running = false;
}
void break_game()
{
	game_running = false;
}

void key_event(int index)
{
	switch (index)
	{
	case main_acti:
		if (ch == 'Q')
		{
			break_main();
		}
		else if ((ch == ' ')
			|| (ch == '\n'))
		{
			game();
		}
		else if ((ch == 'H')
			|| (ch == 'h'))
		{
			helper();
		}
		else if ((ch == 'S')
			|| (ch == 's'))
		{
			setting();
		}
		break;
	
	case game_acti:
		if (ch == 'Q')
		{
			break_game();
			break;
		}
		break;
	}
}


char stage[stage_w][stage_h];
void stage_clean();
void write_game(char [stage_w][stage_h],
		Snake *,
		Food *);
void TB();	//TOB_BOTTOM: 上下边框|
void draw_game(char [stage_w][stage_h]);
char conv_in(char);	//将输入转换为标准方向

void game()
{
	init(game_acti);
	while (game_running)
	{
		get_mtime(&tv, &time2);
		if (time2 - time1 > 300)
		{
			time1 = time2;
			printf(CLEAR);
			show_title();
			show_score();
		
			snake->change_direc(conv_in(ch));	
			snake->forward();
			if (is_snake_ate_food(
				snake->get_head(),
				food->get_pos()))
			{
				if (snake->get_len() == stage_w*stage_h-1)
				{
					is_won = true;
				}
				else
				{
					food->create(snake->get_head());
					score->add(1);
					score->update();
				}
			}
			else
			{
				snake->del_tail();
				if (is_snake_ate_self(snake->get_head()))
				{
					is_died = true;
				}
			}
			
			//开始画
			stage_clean();
			write_game(stage,
				snake,
				food);
			draw_game(stage);
			show_tip(game_acti);
			
			if (is_won || is_died) { break; }
		}
		
		// ch = getin();
		// key_event(ch, game_acti);
	}
	destroy(game_acti);
}


int main(int argc, char* argv[])
{
	init(main_acti);
	
	while (main_running)
	{
		printf(CLEAR);
		
		show_title();
		show_score();
		show_tip(main_acti);
		
		ch = getin();
		key_event(main_acti);
	}
	
	destroy(main_acti);
	return 0;
}


void stage_clean()
{
	for (int i = 0; i < stage_h; i += 1)
	{
		for (int j = 0; j < stage_w; j += 1)
		{
			stage[j][i] = ' ';
		}
	}
}

void write_game(char stage[stage_w][stage_h],
		Snake *snake,
		Food *food)
{
	//把食物写进stage[][];
	stage[food->x][food->y] = food->style;
	
	//把蛇写进stage[][];
	struct Node_st *t1 = snake->get_head();
	while (t1 != null)
	{
		char t2 = snake->style;
		stage[t1->x][t1->y] = t2;
		t1 = t1->next;
	}
}

void TB()
{
	while_run(printf(" #"), stage_w+2);
	printf("\n");
}

void draw_game(char stage[stage_w][stage_h])
{
	TB();
	for (int i = 0; i < stage_h; i += 1)
	{
		for (int j = 0; j < stage_w+1; j += 1)
		{
			if (j == 0)
			{
				printf(" #");
			}
			else if (j == stage_w)
			{
				printf(" #\n");
				//到末尾了, prtf%c会被执行, 
				//但不应执行, 所以跳出
				break;
			}
			
			printf(" %c", stage[j][i]);
		}
	}
	TB();
}

char conv_in(char ch)
{
	if ((ch=='W')
		|| (ch=='w')
		|| (ch=='2')
		|| (ch=='A'))
	{
		return '2';
	}
	else if ((ch=='S')
		|| (ch=='s')
		|| (ch=='8')
		|| (ch=='B'))
	{
		return '8';
	}
	else if ((ch=='A')
		|| (ch=='a')
		|| (ch=='4')
		|| (ch=='D'))
	{
		return '4';
	}
	else if ((ch=='D')
		|| (ch=='d')
		|| (ch=='6')
		|| (ch=='C'))
	{
		return '6';
	}
	
	return '2';
}

