// dear imgui: standalone example application for Android + OpenGL ES 3
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "imgui/imgui.h"
#include "imgui/imgui_impl_android.h"
#include "imgui/imgui_impl_opengl3.h"
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "zj_logger.h"
#define LOGI(...) zj_logger_i("main", __VA_ARGS__)
#define LOGE(...) zj_logger_e("main", __VA_ARGS__)
#if defined(ZJ_DEBUG_MAIN)
#  define LOGD(...) zj_logger_d("main", __VA_ARGS__)
#endif

#include "zj_string.hpp"
zj_string data_path;


// Data
static EGLDisplay           g_EglDisplay = EGL_NO_DISPLAY;
static EGLSurface           g_EglSurface = EGL_NO_SURFACE;
static EGLContext           g_EglContext = EGL_NO_CONTEXT;
static struct android_app*  g_App = NULL;
static bool                 g_Initialized = false;
static char                 g_LogTag[] = "ImGuiExample";

void init(struct android_app* app)
{
    if (g_Initialized)
        return;

    g_App = app;
    ANativeWindow_acquire(g_App->window);

    // Initialize EGL
    // This is mostly boilerplate code for EGL...
    {
        g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (g_EglDisplay == EGL_NO_DISPLAY)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglGetDisplay(EGL_DEFAULT_DISPLAY) returned EGL_NO_DISPLAY");

        if (eglInitialize(g_EglDisplay, 0, 0) != EGL_TRUE)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglInitialize() returned with an error");

        const EGLint egl_attributes[] = { EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE };
        EGLint num_configs = 0;
        if (eglChooseConfig(g_EglDisplay, egl_attributes, nullptr, 0, &num_configs) != EGL_TRUE)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglChooseConfig() returned with an error");
        if (num_configs == 0)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglChooseConfig() returned 0 matching config");

        // Get the first matching config
        EGLConfig egl_config;
        eglChooseConfig(g_EglDisplay, egl_attributes, &egl_config, 1, &num_configs);
        EGLint egl_format;
        eglGetConfigAttrib(g_EglDisplay, egl_config, EGL_NATIVE_VISUAL_ID, &egl_format);
        ANativeWindow_setBuffersGeometry(g_App->window, 0, 0, egl_format);

        const EGLint egl_context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
        g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT, egl_context_attributes);

        if (g_EglContext == EGL_NO_CONTEXT)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglCreateContext() returned EGL_NO_CONTEXT");

        g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, g_App->window, NULL);
        eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Disable loading/saving of .ini file from disk.
    // FIXME: Consider using LoadIniSettingsFromMemory() / SaveIniSettingsToMemory() to save in appropriate location for Android.
    io.IniFilename = NULL;

    // Setup Dear ImGui style
    ImGui::StyleColorsLight();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplAndroid_Init(g_App->window);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Android: The TTF files have to be placed into the assets/ directory (android/app/src/main/assets), we use our GetAssetData() helper to retrieve them.

    // We load the default font with increased size to improve readability on many devices with "high" DPI.
    // FIXME: Put some effort into DPI awareness.
    // Important: when calling AddFontFromMemoryTTF(), ownership of font_data is transfered by Dear ImGui by default (deleted is handled by Dear ImGui), unless we set FontDataOwnedByAtlas=false in ImFontConfig
    // ImFontConfig font_cfg;
    // font_cfg.SizePixels = 32.0f;
    // io.Fonts->AddFontDefault(&font_cfg);
    io.Fonts->AddFontFromFileTTF("/system/fonts/DroidSans.ttf", 32.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    // Arbitrary scale-up
    // FIXME: Put some effort into DPI awareness
    ImGui::GetStyle().ScaleAllSizes(3.0f);

    g_Initialized = true;
}

void tick()
{
    ImGuiIO& io = ImGui::GetIO();
    if (g_EglDisplay == EGL_NO_DISPLAY)
        return;

    // Our state
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Open on-screen (soft) input if requested by Dear ImGui
    static bool WantTextInputLast = false;
    if ((io.WantTextInput && !WantTextInputLast) || (!io.WantTextInput && WantTextInputLast))
        {} //AndroidToggleKeyboard();
    WantTextInputLast = io.WantTextInput;

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("你好世界！HelloWorld!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("这是一些文本。");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("演示窗口", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("另一个窗口", &show_another_window);

        ImGui::SliderFloat("浮点数", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("清屏颜色", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("按钮"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("点击次数 = %d", counter);

        ImGui::Text("程序平均每%.3f毫秒一帧（%.1f FPS）", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("另一个窗口..", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("另一个窗口的问好。");
        if (ImGui::Button("关闭"))
            show_another_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    eglSwapBuffers(g_EglDisplay, g_EglSurface);
}

void shutdown()
{
    if (!g_Initialized)
        return;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();

    if (g_EglDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(g_EglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (g_EglContext != EGL_NO_CONTEXT)
            eglDestroyContext(g_EglDisplay, g_EglContext);

        if (g_EglSurface != EGL_NO_SURFACE)
            eglDestroySurface(g_EglDisplay, g_EglSurface);

        eglTerminate(g_EglDisplay);
    }

    g_EglDisplay = EGL_NO_DISPLAY;
    g_EglContext = EGL_NO_CONTEXT;
    g_EglSurface = EGL_NO_SURFACE;
    ANativeWindow_release(g_App->window);

    g_Initialized = false;
}

static void handleAppCmd(struct android_app* app, int32_t appCmd)
{
    switch (appCmd)
    {
    case APP_CMD_SAVE_STATE:
        break;
    case APP_CMD_INIT_WINDOW:
        init(app);
        break;
    case APP_CMD_TERM_WINDOW:
        shutdown();
        break;
    case APP_CMD_GAINED_FOCUS:
        break;
    case APP_CMD_LOST_FOCUS:
        break;
    }
}

static int32_t handleInputEvent(struct android_app* app, AInputEvent* inputEvent)
{
    if (AKeyEvent_getAction(inputEvent))
    {
        int code = AKeyEvent_getKeyCode(inputEvent);
        int meta_state = AMotionEvent_getMetaState(inputEvent);
        int unicode_key = 0;//AndroidGetUnicodeChar(code, meta_state);
        ImGui::GetIO().AddInputCharacter(unicode_key);
    }
    return ImGui_ImplAndroid_HandleInputEvent(inputEvent);
}

void android_main(struct android_app* app)
{
    app->onAppCmd = handleAppCmd;
    app->onInputEvent = handleInputEvent;

    data_path = zj_string(app->activity->externalDataPath) + '/';
    zj_logger_init(data_path.c_str(), 0);
    zj_logger_open_log();
    zj_logger_open_debug();

    while (true)
    {
        int out_events;
        struct android_poll_source* out_data;

        // Poll all events. If the app is not visible, this loop blocks until g_Initialized == true.
        while (ALooper_pollAll(g_Initialized ? 0 : -1, NULL, &out_events, (void**)&out_data) >= 0)
        {
            // Process one event
            if (out_data != NULL)
                out_data->process(app, out_data);

            // Exit the app by returning from within the infinite loop
            if (app->destroyRequested != 0)
            {
                // shutdown() should have been called already while processing the
                // app command APP_CMD_TERM_WINDOW. But we play save here
                if (!g_Initialized)
                    shutdown();

                return;
            }
        }

        // Initiate a new frame
        tick();
    }

    zj_logger_stop();
}
