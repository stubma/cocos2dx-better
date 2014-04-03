# macros to include all files with same extension

define all-files-under
$(patsubst ./%,%, \
  $(shell cd $(LOCAL_PATH) ; \
          find $(1) -name "$(2)" -and -not -name ".*") \
 )
endef

define all-cpp-files-under
$(call all-files-under,$(1),*.cpp)
endef

define all-c-files-under
$(call all-files-under,$(1),*.c)
endef

# module

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := curl
LOCAL_CFLAGS += -DHAVE_CONFIG_H -DCURL_STATICLIB -DCURL_HIDDEN_SYMBOLS -DBUILDING_LIBCURL -DHTTP_ONLY -fvisibility=hidden
LOCAL_SRC_FILES := $(call all-c-files-under,src)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES) \
	$(LOCAL_PATH)/src \
	$(LOCAL_PATH)/src/vtls
include $(BUILD_STATIC_LIBRARY)
