LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := dear_imgui

ImGui_DIR := ../imgui

# 导出
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(ImGui_DIR)  $(LOCAL_PATH)/$(ImGui_DIR)/backends
LOCAL_EXPORT_LDLIBS     := -lGLESv3 -lEGL -landroid -llog

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(ImGui_DIR)  $(LOCAL_PATH)/$(ImGui_DIR)/backends

# FLAGS
LOCAL_CFLAGS   := -std=c99
LOCAL_CPPFLAGS := -std=c++11

LOCAL_SRC_FILES := \
      $(ImGui_DIR)/imgui.cpp\
      $(ImGui_DIR)/imgui_demo.cpp\
      $(ImGui_DIR)/imgui_draw.cpp\
      $(ImGui_DIR)/imgui_tables.cpp\
      $(ImGui_DIR)/imgui_widgets.cpp\
      $(ImGui_DIR)/backends/imgui_impl_android.cpp\
      $(ImGui_DIR)/backends/imgui_impl_opengl3.cpp

include $(BUILD_STATIC_LIBRARY)
