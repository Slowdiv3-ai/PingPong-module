LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libpingpong
LOCAL_SRC_FILES := pingpong_effect.cpp

# Use LOCAL_LDLIBS for standard NDK libraries
# -lm links the math library (for math.h), -llog links the logging library
LOCAL_LDLIBS := -lm -llog

include $(BUILD_SHARED_LIBRARY)
