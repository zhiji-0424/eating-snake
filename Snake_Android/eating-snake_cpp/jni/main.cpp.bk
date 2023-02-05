#include <EGL/egl.h>
#include <android_native_app_glue.h>

#include "tools/Log.h"
#include "tools/draw/glue.hpp"
#include "snake/Game.hpp"

#define LOGE(...) Log_E("main", __VA_ARGS__)
#if !defined(all_NDEBUG) && !defined(main_cpp_NDEBUG)
#  define LOGD(...) Log_D("main", __VA_ARGS__)
#else
#  define LOGD(...) (void)0
#endif

egl_state egl;
Game game;

static void get_wh(egl_state egl, int32_t& w, int32_t& h)
{
    eglQuerySurface(egl.display, egl.surface, EGL_WIDTH,  &w);
    eglQuerySurface(egl.display, egl.surface, EGL_HEIGHT, &h);
}

void handle_cmd(struct android_app* app, int32_t cmd);
int32_t handle_input(struct android_app* app, AInputEvent *event);

void android_main(struct android_app* app)
{
    Log_init(app->activity->externalDataPath, 0);
#if !defined(all_NDEBUG)
    Log_open_debug();   // 开启调试
#endif
    app->onAppCmd     = handle_cmd;
    app->onInputEvent = handle_input;

    // if (android_app->savedState != nullptr) {}

    while (true) {
        int ident, events;
        struct android_poll_source* source;
        while ((ident=ALooper_pollAll(0, nullptr, &events, (void**)&source)) >= 0) {
            if (source)
                source->process(app, source);
            if (app->destroyRequested)
                return;
        }

        if (!game.is_inited()) game.init(app->activity->externalDataPath);

        game.loop();
        int32_t w, h;
        get_wh(egl, w, h);
        game.draw(w, h, egl);
    }
}

int32_t handle_input(struct android_app* app, AInputEvent* event)
{
    return game.handle_input(app, event);
}

void handle_cmd(struct android_app* app, int32_t cmd)
{
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            glue_init_display(app, &egl);
            game.init_graphics();
            break;
        case APP_CMD_TERM_WINDOW:
            game.term_graphics();
            glue_term_display(&egl);
            break;
    }
}

