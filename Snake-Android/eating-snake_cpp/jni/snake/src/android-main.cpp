#include <imgui.h>
#include <imgui_impl_android.h>
#include <imgui_impl_opengl3.h>

#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include <zj_logger.h>
#define LOG_TAG "android-main"
#define LOGI(...) zj_logger_i(LOG_TAG, __VA_ARGS__)
#define LOGE(...) zj_logger_e(LOG_TAG, __VA_ARGS__)
#if defined(ZJ_DEBUG_MAIN)
#  define LOGD(...) zj_logger_d(LOG_TAG, __VA_ARGS__)
#endif

#include <zj_string.hpp>
#include <zj_android_native_app_glue.hpp>

#include <jni.h>
static void ShowInputDialog(void);
static bool IsInputing(void);
static zj_string GetEditedString(void);
static int GetAssetData(const char* filename, void** out_data);

zj_string data_path;
zj_egl_state egl_state;
struct android_app* app;
ImVec2 touch_pos;

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
    builder.AddText("敬业的语言学家每日委托按下退出你好");
    builder.AddRanges(io.Fonts->GetGlyphRangesDefault()); // Add one of the default ranges
    builder.BuildRanges(&ranges);                          // Build the final result (ordered ranges with all the unique characters submitted)

    zj_string font_file_name = "LXGWWenKai-Regular.ttf";
    //io.Fonts->AddFontFromFileTTF((data_path+font_file_name).c_str(), 40.0f, nullptr, ranges.Data);
    void* file_data;
    int file_size = GetAssetData(font_file_name.c_str(), &file_data);
    io.Fonts->AddFontFromMemoryTTF(file_data, file_size, 40.0f, nullptr, ranges.Data);
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
    zjglue_term_display(&egl_state);    //BUG已修复：HOME键后多任务界面无法再打开此程序
}

// 开始循环
void on_loop()
{
    ImGuiIO& io = ImGui::GetIO();

    bool sdmwd = true;
    ImGui::ShowDemoWindow(&sdmwd);

    ImGui::Begin("每日委托");
    ImGui::Text("敬业的语言学家...");
    ImGui::Text("......hhh.");
    ImGui::Text("=====================");
    if (ImGui::Button("按下退出")) {
        ANativeActivity_finish(app->activity);
    }
    ImGui::End();

    int x = touch_pos.x;
    int y = touch_pos.y;
    ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(x, y), 10, IM_COL32(255, 0, 0, 200), 0);
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


static void ShowInputDialog()
{
    JavaVM* java_vm = app->activity->vm;
    JNIEnv* java_env = NULL;

    jint jni_return = java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6);
    if (jni_return == JNI_ERR) {
        LOGE("ShowInputDialog(): -1");
        return;
    }

    jni_return = java_vm->AttachCurrentThread(&java_env, NULL);
    if (jni_return != JNI_OK) {
        LOGE("ShowInputDialog(): -2");
        return;
    }

    jclass native_activity_clazz = java_env->GetObjectClass(app->activity->clazz);
    if (native_activity_clazz == NULL) {
        LOGE("ShowInputDialog(): -3");
        return;
    }

    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "showInputDialog", "()V");
    if (method_id == NULL) {
        LOGE("ShowInputDialog(): -3");
        return;
    }

    //调用函数
    java_env->CallVoidMethod(app->activity->clazz, method_id);

    jni_return = java_vm->DetachCurrentThread();
    if (jni_return != JNI_OK) {
        LOGE("ShowInputDialog(): -5");
        return;
    }

    //return 0;
}

static bool IsInputing()
{
    JavaVM* java_vm = app->activity->vm;
    JNIEnv* java_env = NULL;

    jint jni_return = java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6);
    if (jni_return == JNI_ERR) {
        LOGE("IsInputing(): -1");
        return false;
    }

    jni_return = java_vm->AttachCurrentThread(&java_env, NULL);
    if (jni_return != JNI_OK) {
        LOGE("IsInputing(): -2");
        return false;
    }

    jclass native_activity_clazz = java_env->GetObjectClass(app->activity->clazz);
    if (native_activity_clazz == NULL) {
        LOGE("IsInputing(): -3");
        return false;
    }

    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "isInputing", "()Z");
    if (method_id == NULL) {
        LOGE("IsInputing(): -3");
        return false;
    }

    //调用函数
    jboolean ret = java_env->CallBooleanMethod(app->activity->clazz, method_id);

    jni_return = java_vm->DetachCurrentThread();
    if (jni_return != JNI_OK) {
        LOGE("IsInputing(): -5");
        return false;
    }

    return (JNI_TRUE==ret) ? true : false;
}

static zj_string GetEditedString()
{
    JavaVM* java_vm = app->activity->vm;
    JNIEnv* java_env = NULL;

    jint jni_return = java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6);
    if (jni_return == JNI_ERR) {
        LOGE("GetEditedString(): -1");
        zj_string e;
        return e;
    }

    jni_return = java_vm->AttachCurrentThread(&java_env, NULL);
    if (jni_return != JNI_OK) {
        LOGE("GetEditedString(): -2");
        zj_string e;
        return e;
    }

    jclass native_activity_clazz = java_env->GetObjectClass(app->activity->clazz);
    if (native_activity_clazz == NULL) {
        LOGE("GetEditedString(): -3");
        zj_string e;
        return e;
    }

    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "getEditedString", "()Ljava/lang/String;");
    if (method_id == NULL) {
        LOGE("GetEditedString(): -3");
        zj_string e;
        return e;
    }

    //调用函数
    jstring jstr = (jstring)java_env->CallObjectMethod(app->activity->clazz, method_id);
    const char* c_str = java_env->GetStringUTFChars(jstr, 0);
    zj_string ret_str = c_str;
    java_env->ReleaseStringUTFChars(jstr, c_str);
    java_env->DeleteLocalRef(jstr);

    jni_return = java_vm->DetachCurrentThread();
    if (jni_return != JNI_OK) {
        LOGE("GetEditedString(): -5");
        zj_string e;
        return e;
    }

    return ret_str;
}

// Helper to retrieve data placed into the assets/ directory (android/app/src/main/assets)
static int GetAssetData(const char* filename, void** outData)
{
    int num_bytes = 0;
    AAsset* asset_descriptor = AAssetManager_open(app->activity->assetManager, filename, AASSET_MODE_BUFFER);
    if (asset_descriptor)
    {
        num_bytes = AAsset_getLength(asset_descriptor);
        *outData = IM_ALLOC(num_bytes);
        int64_t num_bytes_read = AAsset_read(asset_descriptor, *outData, num_bytes);
        AAsset_close(asset_descriptor);
        IM_ASSERT(num_bytes_read == num_bytes);
    }
    return num_bytes;
}
