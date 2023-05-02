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
//Java_net_zhiji_snake_Main_putEditedString(JNIEnv* env, jobject, jstring edited_string) {
static int GetAssetData(const char* filename, void** out_data);
zj_string edited_string;

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
    builder.AddText("敬业的语言学家每日委托按下退出你好");
    builder.AddRanges(io.Fonts->GetGlyphRangesDefault()); // Add one of the default ranges
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

char aaa[8];
// 开始循环
void on_loop()
{
    ImGuiIO& io = ImGui::GetIO();

    // Open on-screen (soft) input if requested by Dear ImGui
    static bool WantTextInputLast = false;
    if (io.WantTextInput && !WantTextInputLast)
        AndroidToggleKeyboard();
    WantTextInputLast = io.WantTextInput;


    bool sdmwd = true;
    ImGui::ShowDemoWindow(&sdmwd);

    //ImGui::Begin("ys");
    ImGui::Begin("每日委托");
    ImGui::Text("敬业的语言学家...");
    ImGui::Text("......hhh.");
    //ImGui::Text("=====================");
    if (ImGui::Button("按下退出")) {
        ANativeActivity_finish(app->activity);
    }
    ImGui::InputText("Hello你好", aaa, IM_ARRAYSIZE(aaa));
    ImGui::End();
    
    int x = touch_pos.x;
    int y = touch_pos.y;
    // LOGI("x: %d, y: %d", x, y);
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
    if (AKeyEvent_getAction(event))
    {
        int code = AKeyEvent_getKeyCode(event);
        int meta_state = AMotionEvent_getMetaState(event);
        int unicode_key = AndroidGetUnicodeChar(code, meta_state);
        ImGui::GetIO().AddInputCharacter(unicode_key);
    }
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


// Helper functions

// Unfortunately, there is no way to show the on-screen input from native code.
// Therefore, we call ShowSoftKeyboardInput() of the main activity implemented in MainActivity.kt via JNI.
/*static int ShowSoftKeyboardInput()
{
    JavaVM* java_vm = app->activity->vm;
    JNIEnv* java_env = NULL;

    jint jni_return = java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6);
    if (jni_return == JNI_ERR)
        return -1;

    jni_return = java_vm->AttachCurrentThread(&java_env, NULL);
    if (jni_return != JNI_OK)
        return -2;

    jclass native_activity_clazz = java_env->GetObjectClass(app->activity->clazz);
    if (native_activity_clazz == NULL)
        return -3;

    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "showSoftInput", "()V");
    if (method_id == NULL)
        return -4;

    java_env->CallVoidMethod(app->activity->clazz, method_id);

    jni_return = java_vm->DetachCurrentThread();
    if (jni_return != JNI_OK)
        return -5;

    return 0;
}

// Unfortunately, the native KeyEvent implementation has no getUnicodeChar() function.
// Therefore, we implement the processing of KeyEvents in MainActivity.kt and poll
// the resulting Unicode characters here via JNI and send them to Dear ImGui.
static int PollUnicodeChars()
{
    JavaVM* java_vm = app->activity->vm;
    JNIEnv* java_env = NULL;

    jint jni_return = java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6);
    if (jni_return == JNI_ERR)
        return -1;

    jni_return = java_vm->AttachCurrentThread(&java_env, NULL);
    if (jni_return != JNI_OK)
        return -2;

    jclass native_activity_clazz = java_env->GetObjectClass(app->activity->clazz);
    if (native_activity_clazz == NULL)
        return -3;

    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "pollUnicodeChar", "()I");
    if (method_id == NULL)
        return -4;

    // Send the actual characters to Dear ImGui
    ImGuiIO& io = ImGui::GetIO();
    jint unicode_character;
    while ((unicode_character = java_env->CallIntMethod(app->activity->clazz, method_id)) != 0)
        io.AddInputCharacter(unicode_character);

    jni_return = java_vm->DetachCurrentThread();
    if (jni_return != JNI_OK)
        return -5;

    return 0;
}*/
/*static void AndroidToggleKeyboard()
{
    JNIEnv *jni;
    app->activity->vm->AttachCurrentThread(&jni, NULL);

    jclass cls = jni->GetObjectClass(app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(cls, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;" );
    jstring service_name = jni->NewStringUTF("input_method");
    jobject input_service = jni->CallObjectMethod(app->activity->clazz, methodID, service_name);

    jclass input_service_cls = jni->GetObjectClass(input_service);
    methodID = jni->GetMethodID(input_service_cls, "toggleSoftInput", "(II)V");
    jni->CallVoidMethod(input_service, methodID, 0, 0);

    jni->DeleteLocalRef(service_name);

    app->activity->vm->DetachCurrentThread();
}

static int AndroidGetUnicodeChar( int keyCode, int metaState )
{
    //https://stackoverflow.com/questions/21124051/receive-complete-android-unicode-input-in-c-c/43871301

    int eventType = AKEY_EVENT_ACTION_DOWN;
    JNIEnv *jni;
    app->activity->vm->AttachCurrentThread(&jni, NULL);

    jclass class_key_event = jni->FindClass("android/view/KeyEvent");

    jmethodID method_get_unicode_char = jni->GetMethodID(class_key_event, "getUnicodeChar", "(I)I");
    jmethodID eventConstructor = jni->GetMethodID(class_key_event, "<init>", "(II)V");
    jobject eventObj = jni->NewObject(class_key_event, eventConstructor, eventType, keyCode);

    int unicodeKey = jni->CallIntMethod(eventObj, method_get_unicode_char, metaState );

    app->activity->vm->DetachCurrentThread();

    return unicodeKey;
}
*/

extern "C" JNIEXPORT void JNICALL
Java_net_zhiji_snake_Main_putEditedString(JNIEnv* env, jobject, jstring edited_string) {
    //if (::edited_stringd.size()!=0) ::edited_string = "";
    const char* c_edited_string = env->GetStringUTFChars(edited_string, 0);
    ::edited_string == c_edited_string;
    env->ReleaseStringUTFChars(edited_string, c_edited_string);
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
