#LOCAL_PATH := $(call my-dir)

#include $(CLEAR_VARS)

#LOCAL_MODULE    := RakNet
#LOCAL_SRC_FILES := RakNet.cpp

#include $(BUILD_SHARED_LIBRARY)

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := RakNet
MY_PREFIX := $(LOCAL_PATH)/RakNetSources/

MY_SOURCES := $(wildcard jni/RakNetSources/*.cpp)

LOCAL_SRC_FILES += $(MY_SOURCES:$(MY_PREFIX)%=RakNetSources/%)
LOCAL_SRC_FILES += RakNet.cpp
$(warning $(LOCAL_SRC_FILES))
include $(BUILD_SHARED_LIBRARY)