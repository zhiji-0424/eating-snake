#if !defined(ZJ_ANDROID_NATIVE_APP_GLUE_HPP)
#define ZJ_ANDROID_NATIVE_APP_GLUE_HPP

// 专为NativeActivity准备

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android_native_app_glue.h>
#include "../common/zj_logger.h"

struct zj_egl_state {
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
};

// 请在Native窗口更新时调用(遇到APP_COMMAND_XXXX_WINDOW时)
extern "C" {
    int  zjglue_init_display(struct android_app* app, struct zj_egl_state* egl);
    void zjglue_term_display(struct zj_egl_state* egl);
}

#endif
