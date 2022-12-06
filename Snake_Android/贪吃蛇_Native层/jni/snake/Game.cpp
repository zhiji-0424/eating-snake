#include "Game.hpp"
#include <tools/String.hpp>
#include <tools/draw/draw.hpp>

#include <stdlib.h>
#include <sys/time.h>
#include "food.h"
#include "score.h"
#include "snake.h"
    
#define LOGE(...) Log_E("Game", __VA_ARGS__)
#if !defined(NDEBUG) && !defined(main_NDEBUG)
#  define LOGD(...) Log_D("Game", __VA_ARGS__);
#else
#  define LOGD(...) (void)0
#endif

#define stage_w 30          // 蛇活动空间大小(方格数量)
#define stage_h 50          // 蛇活动空间大小(方格数量)
float block_w;            // 一个方格的大小
float dx, dy;             // 横向竖向偏移
float stageW, stageH;     // 活动空间像素大小

size_t speed;               // 速度(蛇两次前进的时间间隔)
int32_t ix, iy;             // 手指的位置

// 获取系统时间(精确到0.001秒)
static void get_time(size_t& rs)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    rs = tv.tv_sec*1000 + tv.tv_usec/1000;
}

static void create_food_2()
{
    do
        create_food(stage_w, stage_h);
    while (food_in_snake());
}

Game::Game()
{
    inited = false;
    drawing = false;
}

Game::~Game()
{
}

// 做绘图准备
void Game::init_graphics()
{
    // 初始化绘图功能
    draw_init();
    drawing = true;
}

// 关闭绘图
void Game::term_graphics()
{
    drawing = false;
    draw_destroy();
}

// 初始化工作
void Game::init(const char* externalDataPath)
{
    if (!inited) {
        inited = true;
        data_path = String(externalDataPath) + '/';
    }
}

bool game_inited  = false;
bool game_running = false;
size_t time1 = 0, time2 = 0;
int32_t direc = up;

void game_init() {
    snake_t* snake = get_snake();
    snake->x = stage_w / 2;
    snake->y = stage_h / 2;
    snake->direction = up;
    while (snake->length != 1) {
        // 蛇身归零
        snake_del_tail();
    }
    
    srand(time(NULL));
    create_food_2();
}

void Game::loop() {
    if (!game_inited) {
        game_init();
        game_inited = true;
    }
    if (game_running) {
        get_time(time2);
        if (time2 - time1 > speed) {
            time1 = time2;
            /* 解释蛇移动：
             * 首先向前一步，这时候尾巴不动(相当于头伸出来了)，
             *   吃到食物就不用删掉末节尾巴，但要刷新食物，
             *   没有吃到就删掉末节尾巴
             * 吃到了自己那就游戏结束。 */
            snake_change_direc(direc);
            snake_forward(stage_w, stage_h);
            if (snake_ate_food()) {
                create_food_2();
            } else {
                snake_del_tail();
            }
            if (snake_ate_self()) {
                game_running = false;
                game_inited  = false;
            }
        }
    }
}

// 画出边框
void draw_stage()
{
    set_color(1, 1, 1);
    draw_line(dx, dy, dx+stageW, dy);               // 上横
    draw_line(dx, dy+stageH, dx+stageW, dy+stageH); // 下横
    draw_line(dx, dy, dx, dy+stageH);               // 左竖
    draw_line(dx+stageW, dy, dx+stageW, dy+stageH); // 右竖
}

void draw_food()
{
    food_t food = get_food();
    set_color(1, 0, 0);         // 食物用红色
    draw_rect(food.x*block_w+dx, (stage_h-1-food.y)*block_w+dy, block_w, block_w);
}

void draw_snake()
{
    snake_t* node = get_snake();
    set_color(1, 1, 1);         // 蛇身用白色
    while (node) {
        draw_rect(node->x*block_w+dx, (stage_h-1-node->y)*block_w+dy, block_w, block_w);
        node = node->next;
    }
}

void Game::draw(int32_t w, int32_t h, egl_state egl)
{
    if (!drawing) return;
    set_scrwh(w, h);
    draw_color(0, 0, 0);
    set_color(1, 1, 1);

    /* 硬编码(要偏移是为了避免屏幕玻璃厚度影响：
             比如使用量筒时那样) */
    dx = w * 0.025;
    block_w = (w - 2*dx) / stage_w;
    stageW = block_w * stage_w;
    stageH = block_w * stage_h;
    dy = (h - stageH) / 2;

    draw_stage();
    draw_food();
    draw_snake();
    
    draw_circle_line(ix, iy, 10, 1);    // 手指轨迹

    eglSwapBuffers(egl.display, egl.surface);
}

int32_t last_x, last_y;     // 手指上一次位置
int32_t now_x, now_y;       // 手指现在位置
size_t last_time;       // 上一次系统时刻
size_t now_time;        // 现在系统时刻

int32_t Game::handle_input(struct android_app* app, AInputEvent* event)
{
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
        if (AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_UP) {
            // 返回键按下时
            game_running = false;
            game_inited  = false;
            drawing = false;
            game_init();
        }
    }
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        ix = AMotionEvent_getX(event, 0);
        iy = AMotionEvent_getY(event, 0);
        speed = 150;    // 一般速度
        if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_DOWN) {
            game_running = true;
            last_x = ix;
            last_y = iy;
            get_time(last_time);
            if (iy > dy+stageH) {
                // 手指Y坐标在贪吃蛇活动空间下
                speed = 10;     // 加速
            }
        }
        if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_UP) {
            now_x = ix;
            now_y = iy;
            get_time(now_time);
            if (now_time-last_time <= 250) {
                int32_t deta_x = now_x-last_x;
                int32_t deta_y = now_y-last_y;
                if (abs(deta_x) > abs(deta_y)) {
                    direc = (deta_x > 0) ? right : left;
                } else if (abs(deta_x) < abs(deta_y)) {
                    direc = (deta_y > 0) ? down : up;
                }
            }
        }
    }
    return 0;
}



