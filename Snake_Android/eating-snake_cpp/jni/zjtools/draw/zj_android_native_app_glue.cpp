#include "zj_android_native_app_glue.hpp"

#define LOGE(...) zj_logger_e("zj_android_native_app_glue", __VA_ARGS__);
#if defined(ZJ_DEBUG_ALL) || defined(ZJ_DEBUG_GLUE)
#  define LOGD(...) zj_logger_d("zj_android_native_app_glue", __VA_ARGS__)
#else
#  define LOGD(...) (void)0
#endif

int zjglue_init_display(struct android_app* app, struct zj_egl_state* egl)
{
    if (app->window == nullptr) {
        return -1;
    }
    const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_DEPTH_SIZE, 24,
        EGL_BLUE_SIZE,   8,
        EGL_GREEN_SIZE,  8,
        EGL_RED_SIZE,    8,
        EGL_NONE
    };
    const EGLint context_attributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };
    EGLint format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, app->window, nullptr);
    context = eglCreateContext(display, config, nullptr, context_attributes);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGE("Unable to eglMakeCurrent");
        return -2;
    }

    egl->display = display;
    egl->context = context;
    egl->surface = surface;

    // Initialize GL state.
    // glEnable(GL_CULL_FACE);
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void zjglue_term_display(struct zj_egl_state* egl)
{
    if (egl->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(egl->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (egl->context != EGL_NO_CONTEXT)
            eglDestroyContext(egl->display, egl->context);
        if (egl->surface != EGL_NO_SURFACE)
            eglDestroySurface(egl->display, egl->surface);
        eglTerminate(egl->display);
    }
    egl->display = EGL_NO_DISPLAY;
    egl->context = EGL_NO_CONTEXT;
    egl->surface = EGL_NO_SURFACE;
}