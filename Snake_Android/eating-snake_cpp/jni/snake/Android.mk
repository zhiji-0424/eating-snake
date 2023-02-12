LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(ImGui_DIR) $(ImGui_DIR/backends) $(zjtools_DIR/jni/src)
LOCAL_STATIC_LIBRARIES := android_native_app_glue dear_imgui zjtools
LOCAL_MODULE := main
LOCAL_LDLIBS := -landroid -llog -lGLESv3 -lEGL

#源文件
LOCAL_SRC_FILES := \
    src/main.cpp

#开始构建
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)