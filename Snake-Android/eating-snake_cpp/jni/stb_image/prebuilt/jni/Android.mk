LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

MY_MICRO := -DSTBI_ONLY_BMP -DSTBI_ONLY_PNG -DSTBI_ONLY_JPEG

MY_FLAGS := $(MY_MICRO)

# FLAGS
LOCAL_CFLAGS   := -std=c99      $(MY_MICRO)
LOCAL_CPPFLAGS := -std=c++11    $(MY_MICRO)

LOCAL_MODULE := stb_image

STB_IMAGE_DIR := ../../../../../../Snake-core/stb_image/

#源文件
LOCAL_SRC_FILES := $(STB_IMAGE_DIR)/stb_image.cpp

#开始构建
include $(BUILD_STATIC_LIBRARY)
