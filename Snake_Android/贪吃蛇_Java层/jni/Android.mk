LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS    := -std=gnu11   -Wall -Wextra
LOCAL_CPPFLAGS  := -std=gnu++11 -Wall -Wextra
LOCAL_LDLIBS    := -llog -landroid -ldl
LOCAL_MODULE    := main
LOCAL_SRC_FILES := main.c
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
