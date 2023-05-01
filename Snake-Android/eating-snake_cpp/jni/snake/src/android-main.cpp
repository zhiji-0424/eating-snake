#include <imgui.h>
#include <imgui_impl_android.h>
#include <imgui_impl_opengl3.h>

#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include <zj_logger.h>
#define LOGI(...) zj_logger_i("main", __VA_ARGS__)
#define LOGE(...) zj_logger_e("main", __VA_ARGS__)
#if defined(ZJ_DEBUG_MAIN)
#  define LOGD(...) zj_logger_d("main", __VA_ARGS__)
#endif

#include <zj_string.hpp>
#include <zj_android_native_app_glue.hpp>


zj_string data_path;
zj_egl_state egl_state;
ImVec2 touch_pos;
struct android_app* app;

//平面直角坐标系
//绘图API包装
//跨平台编译

// 当窗口重开时
void init_display(struct android_app* app)
{
    ::app = app;
    // Initialize EGL
    zjglue_init_display(app, &egl_state);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = NULL;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplAndroid_Init(app->window);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    ImVector<ImWchar> ranges;
    ImFontGlyphRangesBuilder builder;
    builder.AddText("敬业的语言学家每日委托按下退出");
    builder.AddText("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 .,?!@\"\':;…+-*/=~()[]{}<>([{<>}])|_\\^`&#%$·");                        // Add a string (here "Hello world" contains 7 unique characters)
    // builder.AddRanges(io.Fonts->GetGlyphRangesChineseSimplifiedCommon()); // Add one of the default ranges
    builder.BuildRanges(&ranges);                          // Build the final result (ordered ranges with all the unique characters submitted)

    io.Fonts->AddFontFromFileTTF("/system/fonts/NotoSansCJK-Regular.ttc", 40.0f, nullptr, ranges.Data);
    io.Fonts->Build();                                     // Build the atlas while 'ranges' is still in scope and not deleted.

    // Arbitrary scale-up
    ImGui::GetStyle().ScaleAllSizes(1.0f);
}

// 当窗口关闭时
void term_display()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
}

// 开始循环
void on_loop()
{
    //ImGui::Begin("ys");
    ImGui::Begin("每日委托");
    ImGui::Text("敬业的语言学家...");
    ImGui::Text("......hhh.");
    //ImGui::Text("=====================");
    if (ImGui::Button("按下退出")) {
        ANativeActivity_finish(app->activity);
    }
    ImGui::End();
    
    int x = touch_pos.x;
    int y = touch_pos.y;
    // LOGI("x: %d, y: %d", x, y);
    ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(x, y), 10, IM_COL32(255, 0, 0, 200), 0);
}

// 对 on_loop 的包装
void tick()
{
    if (egl_state.display == EGL_NO_DISPLAY)
        return;
    ImGuiIO& io = ImGui::GetIO();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    on_loop();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0.2f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    eglSwapBuffers(egl_state.display, egl_state.surface);
}

static void handle_cmd(struct android_app* app, int32_t appCmd)
{
    switch (appCmd)
    {
        case APP_CMD_INIT_WINDOW:
            init_display(app);
            break;
        case APP_CMD_TERM_WINDOW:
            term_display();
            break;
    }
}

static int32_t handle_input(struct android_app* app, AInputEvent* event)
{
    switch (AInputEvent_getType(event)) {
        case AINPUT_EVENT_TYPE_MOTION: {
            touch_pos.x = AMotionEvent_getX(event, 0);
            touch_pos.y = AMotionEvent_getY(event, 0);
        }
    }
    return ImGui_ImplAndroid_HandleInputEvent(event);
}

void android_main(struct android_app* app)
{
    app->onAppCmd     = handle_cmd;
    app->onInputEvent = handle_input;

    data_path = zj_string(app->activity->externalDataPath) + "/";
    zj_logger_init(data_path.c_str(), 0);
    zj_logger_open_log();
    zj_logger_open_debug();

    while (true)
    {
        int out_events;
        struct android_poll_source* out_data;

        while (ALooper_pollAll(0, NULL, &out_events, (void**)&out_data) >= 0)
        {
            // Process one event
            if (out_data != NULL)
                out_data->process(app, out_data);

            if (app->destroyRequested != 0)
                return;
        }

        // Initiate a new frame
        tick();
    }

    zj_logger_stop();
}
