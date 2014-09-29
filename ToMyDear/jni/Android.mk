LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE := RakNet 
MY_PREFIX := $(LOCAL_PATH)/RakNetSources/ 
MY_SOURCES := $(wildcard $(MY_PREFIX)*.cpp) 
LOCAL_SRC_FILES += $(MY_SOURCES:$(MY_PREFIX)%=RakNetSources/%)

LOCAL_LDLIBS := -landroid -llog
include $(BUILD_SHARED_LIBRARY)

