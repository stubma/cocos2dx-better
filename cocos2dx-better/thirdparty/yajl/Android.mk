###########################################################
# build libyajl
###########################################################

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := yajl
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES) \
	$(LOCAL_PATH)/src
LOCAL_SRC_FILES := $(call all-c-files-under,.)

include $(BUILD_STATIC_LIBRARY)