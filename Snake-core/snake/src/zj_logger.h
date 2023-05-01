#if !defined(ZJ_LOGGER_H)
#define ZJ_LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#if 0
/*  一般用法:
 *      zj_logger_init("/sdcard/", 0);   //不管什么情况都用这个启动
 *      zj_logger_open_debug();       //调试时启用(不调试的话就不写这句)
 *      
 *      zj_logger_I(TAG1, "Application started successful");  //ok
 *      zj_logger_D(TAG2, "The address of var is %p", &var);  //ok
 *      可以使用 zj_logger_open/stop_xxx() 来临时开启/关闭Log。
 *  退出:
 *      zj_logger_stop();
 */
#endif

/* 初始化(设定日志文件路径, 文件名默认是“run_time.log”)
 * file_name 传0则使用默认名字。 */
void zj_logger_init(const char* file_dir_path, const char* file_name);

// 停止
void zj_logger_stop(void);

// 获取文件完整路径
const char* zj_logger_get_file_path(void);

// 基本日志记录
void zj_logger_open_log(void);
void zj_logger_stop_log(void);
int  zj_logger_is_opened(void);

// 调试模式
void zj_logger_open_debug(void);
void zj_logger_stop_debug(void);
int  zj_logger_is_debugging(void);

// 它们的参数与printf()类似, 优先级从上往下递减(EWIDV)
void zj_logger_e(const char* tag, const char* fmt, ...);
void zj_logger_w(const char* tag, const char* fmt, ...);
void zj_logger_i(const char* tag, const char* fmt, ...);
void zj_logger_d(const char* tag, const char* fmt, ...);  //需要zj_logger_open_debug()才能显示;
void zj_logger_v(const char* tag, const char* fmt, ...);  //需要zj_logger_open_debug()才能显示;

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //ZJ_LOGGER_H
