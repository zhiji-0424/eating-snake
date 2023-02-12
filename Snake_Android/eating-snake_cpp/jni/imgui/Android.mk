LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := dear_imgui

ImGui_DIR := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
      $(ImGui_DIR)/imgui.cpp\
      $(ImGui_DIR)/imgui_demo.cpp\
      $(ImGui_DIR)/imgui_draw.cpp\
      $(ImGui_DIR)/imgui_tables.cpp\
      $(ImGui_DIR)/imgui_widgets.cpp\
      $(ImGui_DIR)/backends/imgui_impl_android.cpp\
      $(ImGui_DIR)/backends/imgui_impl_opengl3.cpp

include $(BUILD_STATIC_LIBRARY)