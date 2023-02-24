LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

MY_MICRO := -DZJ_USES_ANDROID_NATIVE_APP_GLUE

MY_FLAGS := -Wall -Wextra $(MY_MICRO)

# FLAGS
LOCAL_CFLAGS   := -std=c99      $(MY_MICRO)
LOCAL_CPPFLAGS := -std=c++11    $(MY_MICRO)

LOCAL_STATIC_LIBRARIES := android_native_app_glue dear_imgui zjtools
LOCAL_MODULE := main

#源文件
LOCAL_SRC_FILES := \
    src/main.cpp

#开始构建
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)