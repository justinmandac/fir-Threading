LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := buffer
LOCAL_SRC_FILES := com_fir_firBuff.c
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)