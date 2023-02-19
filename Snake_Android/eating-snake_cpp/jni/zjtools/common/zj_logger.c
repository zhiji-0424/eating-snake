// Last edit time is 2023-01-13 23:49:51. By zhiji 
#include "zj_logger.h"

#define zj_logger_ON  1       //开启
#define zj_logger_OFF 0       //关闭
#define FILE_PATH_LEN 1024
#define FILE_NAME_LEN 100
#define Log_BUF_LEN   200

static char file_path[FILE_PATH_LEN];
static int  log_mode   = zj_logger_OFF;
static int  debug_mode = zj_logger_OFF;

static int is_correct_str(const char* str)
{
    return (str != NULL) && (str[0] != ' ');
}

void zj_logger_init(const char* file_dir_path, const char* file_name)
{
    if (!is_correct_str(file_name)) {
        file_name = ZJ_DEFAULT_FILE_NAME;
    }
    if (is_correct_str(file_dir_path)) {
        sprintf(file_path, "%s/%s", file_dir_path, file_name);
        zj_logger_open_log();
        zj_logger_stop_debug();
    }
}

void zj_logger_stop()
{
    zj_logger_stop_debug();
    zj_logger_stop_log();
}

const char* zj_logger_get_file_path()
{
    return file_path;
}

void zj_logger_open_log(void)
{
    log_mode = zj_logger_ON;
}

void zj_logger_stop_log(void)
{
    log_mode = zj_logger_OFF;
}

int  zj_logger_is_opened(void)
{
    return log_mode == zj_logger_ON;
}

void zj_logger_open_debug(void)
{
    debug_mode = zj_logger_ON;
}

void zj_logger_stop_debug(void)
{
    debug_mode = zj_logger_OFF;
}

int zj_logger_is_debugging(void)
{
    return debug_mode == zj_logger_ON;
}

static void zj_logger_writer(const char* type, const char* tag, const char* fmt, va_list args)
{
    FILE* f = fopen(file_path, "a");
    if (f==NULL) return;

    static char fmt2[Log_BUF_LEN];
    time_t now = time(NULL);
    char time_str[30];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    if (tag == NULL) tag = "unknown";
    sprintf(fmt2, "[%s] [%s] %s | %s\n", time_str, type, tag, fmt);
    
    vfprintf(f, fmt2, args);
    fclose(f);
}

void zj_logger_e(const char* tag, const char* fmt, ...)
{
    if (zj_logger_is_opened()) {
        va_list args;
        va_start(args, fmt);
        zj_logger_writer("E/ERROR", tag, fmt, args);
        va_end(args);
    }
}

void zj_logger_w(const char* tag, const char* fmt, ...)
{
    if (zj_logger_is_opened()) {
        va_list args;
        va_start(args, fmt);
        zj_logger_writer("W/WARRING", tag, fmt, args);
        va_end(args);
    }
}

void zj_logger_i(const char* tag, const char* fmt, ...)
{
    if (zj_logger_is_opened()) {
        va_list args;
        va_start(args, fmt);
        zj_logger_writer("I/INFO", tag, fmt, args);
        va_end(args);
    }
}

void zj_logger_d(const char* tag, const char* fmt, ...)
{
    if (zj_logger_is_debugging()) {
        va_list args;
        va_start(args, fmt);
        zj_logger_writer("D/DEBUG", tag, fmt, args);
        va_end(args);
    }
}

void zj_logger_v(const char* tag, const char* fmt, ...)
{
    if (zj_logger_is_debugging()) {
        va_list args;
        va_start(args, fmt);
        zj_logger_writer("V/VERBOSE", tag, fmt, args);
        va_end(args);
    }
}
