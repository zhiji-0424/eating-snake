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
static const char* main_func_name = "android_main";
static const char* exPath;
static const char* inPath;

void android_main(struct android_app* app) {
    exPath = app->activity->externalDataPath;
    inPath = app->activity->internalDataPath;
    sprintf(log_file_name, "%s/starter.log", exPath);

    char* msg = NULL;
    char lib_path[150];
    // 指定库的路径并打开
    sprintf(lib_path, "%s/libmain.so", inPath);
    void* lib = dlopen(lib_path, RTLD_NOW);
    if ((msg=dlerror(),msg)) {
        log_cat(msg);
        return;
    }
    // 解析函数并运行
    MainFunc f = (MainFunc)dlsym(lib, main_func_name);
    if ((msg=dlerror(),msg)) {
        log_cat(msg);
        return;
    }
    f(app);
    dlclose(lib);
}