LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

MY_DEBUG_MODE := 

MY_MICRO := $(MY_DEBUG_MODE) \
    -DString_NDEBUG \
    -DSTBI_ONLY_BMP -DSTBI_ONLY_PNG -DSTBI_ONLY_JPEG 

MY_INCLUDE_PATH := /sdcard/games/.tyri/workspace/Android/MyAPIs/
MY_TOOLS_DIR := $(MY_INCLUDE_PATH)/tools/

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(MY_INCLUDE_PATH)
LOCAL_CFLAGS   := -std=c99   -Wall -Wextra $(MY_MICRO)
LOCAL_CPPFLAGS := -std=c++11 -Wall -Wextra $(MY_MICRO)
LOCAL_LDLIBS   := -llog -landroid -lGLESv3 -lEGL

LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_MODULE := main

LOCAL_SRC_FILES := \
    main.cpp \
    $(MY_TOOLS_DIR)/Log.c  $(MY_TOOLS_DIR)/String.cpp \
    $(MY_TOOLS_DIR)/draw/glue.cpp  $(MY_TOOLS_DIR)/draw/Shader.cpp  $(MY_TOOLS_DIR)/draw/draw.cpp  $(MY_TOOLS_DIR)/draw/stb_image.cpp \
    snake/Game.cpp snake/food.c snake/score.c snake/snake.c
    

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)