#include <stdlib.h>
#include <dlfcn.h>
#include <android_native_app_glue.h>

static char log_file_name[150];
static void log_cat(const char *msg) {
    FILE* fp = fopen(log_file_name, "a");
    if (!fp) {
        return;
    }
    fprintf(fp, "%s\n", msg);
    fclose(fp);
}

typedef void (*MainFunc)(struct android_app*);
static const char* exPath;
static const char* inPath;

#define OK  1
#define ERR 0
static int check(void)
{
    char* msg = dlerror();
    if (msg != NULL) {
        // 有错误信息
        log_cat(msg);
        return ERR;
    }
    return OK;
}

void android_main(struct android_app* app)
{
    exPath = app->activity->externalDataPath;
    inPath = app->activity->internalDataPath;

    // 指定日志位置
    sprintf(log_file_name, "%s/loader.log", exPath);

    // 复制库文件
    char cmd[2000];
    sprintf(cmd, "cp %s/libmain.so %s/libmain.so 2>> %s", exPath, inPath, log_file_name);
    system(cmd);

    // 指定库的路径
    char lib_path[150];
    sprintf(lib_path, "%s/libmain.so", inPath);

    // 读取库文件
    void* lib = dlopen(lib_path, RTLD_NOW);
    if (check() == ERR)
        return;

    // 解析函数并运行
    MainFunc f = (MainFunc)dlsym(lib, "android_main");
    if (check() == OK) {
        f(app);
    }

    // 关闭库文件
    dlclose(lib);
}