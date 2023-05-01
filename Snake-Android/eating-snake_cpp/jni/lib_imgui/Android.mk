LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := dear_imgui
LOCAL_SRC_FILES := ./prebuilt/obj/local/$(TARGET_ARCH_ABI)/libdear_imgui.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../Snake-core/imgui-1.89.4  $(LOCAL_PATH)/../../../../Snake-core/imgui-1.89.4/backends
LOCAL_EXPORT_LDLIBS     := -lGLESv3 -lEGL -landroid -llog
include $(PREBUILT_STATIC_LIBRARY)