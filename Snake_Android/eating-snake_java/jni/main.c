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

static int32_t handle_input(struct android_app* app, AInputEvent *event)
{
    // 退出
    ANativeActivity_finish(app->activity);
    return 1;
}

static void handle_cmd(struct android_app* app, int32_t cmd)
{
}

static void loop(struct android_app* app)
{
    app->onAppCmd     = handle_cmd;
    app->onInputEvent = handle_input;
    app->userData     = 0;
    while (1) {
        int ident, events;
        struct android_poll_source* source;
        while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) {
            if (source) source->process(app, source);
            if (app->destroyRequested) return;
        }
    }
}

void android_main(struct android_app* app)
{
    exPath = app->activity->externalDataPath;
    inPath = app->activity->internalDataPath;

    // 指定日志位置
    sprintf(log_file_name, "%s/loader.log", exPath);
    log_cat("");    // 分割上一次的信息

    // 复制库文件
    char cmd[2000];
    sprintf(cmd, "cp %s/libmain.so %s/libmain.so 2>> %s", exPath, inPath, log_file_name);
    log_cat(cmd);
    system(cmd);

    // 指定库的路径
    char lib_path[150];
    sprintf(lib_path, "%s/libmain.so", inPath);

    // 读取库文件
    void* lib = dlopen(lib_path, RTLD_NOW);
    if (check() == OK) {
        // 解析函数并运行
        MainFunc f = (MainFunc)dlsym(lib, "android_main");
        if (check() == OK) {
            f(app);
        } else {
            loop(app);
        }
    } else {
        loop(app);
    }

    // 关闭库文件
    dlclose(lib);
}

