LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libpingpong
LOCAL_SRC_FILES := pingpong_effect.cpp
LOCAL_C_INCLUDES := $(call include-path-for, audio-effects)
LOCAL_SHARED_LIBRARIES := liblog libcutils libutils
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES

include $(BUILD_SHARED_LIBRARY)
