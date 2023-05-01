LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

MY_MICRO := 

MY_FLAGS := -Wall -Wextra $(MY_MICRO)

# FLAGS （可改）
LOCAL_CFLAGS   := -std=c99   $(MY_FLAGS)
LOCAL_CPPFLAGS := -std=c++11 $(MY_FLAGS)

# 源代码位置
MY_SRC_DIR := ./

# 与 stb_image 有关的
LOCAL_MODULE := stb_image
LOCAL_SRC_FILES := $(MY_SRC_DIR)/draw/stb_image.cpp
include $(BUILD_STATIC_LIBRARY)

# 模块名字
LOCAL_MODULE := zjtools
LOCAL_MODULE_FILENAME := libzjtools

# 需要的库
LOCAL_STATIC_LIBRARIES := android_native_app_glue stb_image

# 源代码
LOCAL_SRC_FILES :=  #弃用$(MY_SRC_DIR)/draw/zj_draw.cpp\
                    $(MY_SRC_DIR)/draw/zj_android_native_app_glue.cpp\
                    $(MY_SRC_DIR)/draw/zj_shader.cpp\
                    $(MY_SRC_DIR)/common/zj_string.cpp\
                    $(MY_SRC_DIR)/common/zj_utf8_string.cpp\
                    $(MY_SRC_DIR)/common/zj_utf8_to_utf16.c\
                    $(MY_SRC_DIR)/common/zj_logger.c

# 导出
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(MY_SRC_DIR)
LOCAL_EXPORT_LDLIBS     := -lGLESv3 -lEGL -landroid

# 开始构建
include $(BUILD_STATIC_LIBRARY)
