/*
 *  Copyright (c) 2022-2025 zhiji_0424
 *  eating-snake is licensed under Mulan PSL v2.
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 
 *  You may obtain a copy of Mulan PSL v2 at:
 *           http://license.coscl.org.cn/MulanPSL2 
 *  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  
 *  See the Mulan PSL v2 for more details.  
 */
/*
#include "main.h"

#define main_acti 0
#define game_acti 1

#define title "====eating-snake===="

//毫秒级时间戳
struct timeval tv;
long int time1 = 0;
long int time2 = 0;


int main(void)
{

    return 0;
}
*/

#include <ncurses.h>
#include <sys/param.h>

int main()
{
	initscr();
	printw("Press WASD to move the cross, Q to quit\n");
	int x = 10;
	int y = 10;
	refresh();
	int ch = 0;
	noecho();
	while (ch != 'Q')
	{
		move(1, 0);
		printw("Posithio:(%d, %d)", x, y);
		ch = getch();
		int sy, sx;
		getmaxyx(stdscr, sy, sx);
		move(y, x);
		delch();
		if (ch == '2')
			y--;
		if (ch == '8')
			y++;
		if (ch == '4')
			x--;
		if (ch == '6')
			x++;
		y = MAX(1, MIN(sy - 1, y));
		x = MAX(1, MIN(sx - 1, x));
		move(y, x);
		addch('@');
		refresh();
	}
	endwin();
	return 0;
}
