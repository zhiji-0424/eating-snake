LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := stb_image
LOCAL_SRC_FILES := ./prebuilt/obj/local/$(TARGET_ARCH_ABI)/libstb_image.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../Snake-core/stb_image
include $(PREBUILT_STATIC_LIBRARY)