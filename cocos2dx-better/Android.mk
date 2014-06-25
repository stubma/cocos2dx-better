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

COCOS2DX_ROOT=$(LOCAL_PATH)/../../cocos2d-x
COCOS2DX_EXT_ROOT=$(COCOS2DX_ROOT)/extensions
COCOSDENSHION_ROOT=$(COCOS2DX_ROOT)/CocosDenshion
LOCAL_MODULE := cocos2dx-better
ifeq ($(NDK_TOOLCHAIN_VERSION),clang)
	LOCAL_CFLAGS += -Qunused-arguments -Wno-unknown-warning-option -Wno-c++11-extensions
endif
LOCAL_EXPORT_CFLAGS := $(LOCAL_CFLAGS)
LOCAL_SRC_FILES := $(call all-cpp-files-under,src) \
	$(call all-c-files-under,$(COCOS2DX_ROOT)/scripting/lua/lua) \
	$(call all-c-files-under,$(COCOS2DX_ROOT)/scripting/lua/tolua) \
	$(call all-cpp-files-under,$(COCOS2DX_EXT_ROOT)/CocoStudio/) \
	$(call all-cpp-files-under,$(COCOS2DX_EXT_ROOT)/GUI/CCControlExtension) \
	$(call all-cpp-files-under,$(COCOS2DX_EXT_ROOT)/GUI/CCScrollView) \
	$(COCOS2DX_EXT_ROOT)/GUI/CCEditBox/CCEditBox.cpp \
	$(COCOS2DX_EXT_ROOT)/GUI/CCEditBox/CCEditBoxImplAndroid.cpp \
	$(COCOS2DX_EXT_ROOT)/CCBReader/CCData.cpp
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include \
	$(LOCAL_PATH)/include/platform/android \
	$(LOCAL_PATH)/src/tolua_pkg \
	$(COCOS2DX_ROOT)/scripting/lua/lua \
	$(COCOS2DX_ROOT)/scripting/lua/tolua \
	$(COCOS2DX_ROOT)/cocos2dx/platform/android \
	$(COCOS2DX_ROOT)/cocos2dx/platform/android/jni \
	$(COCOSDENSHION_ROOT)/include \
	$(COCOS2DX_EXT_ROOT) \
	$(COCOS2DX_EXT_ROOT)/GUI/CCControlExtension \
	$(COCOS2DX_EXT_ROOT)/GUI/CCEditBox \
	$(COCOS2DX_EXT_ROOT)/GUI/CCScrollView \
	$(COCOS2DX_EXT_ROOT)/CCBReader \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Action \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Armature \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Armature/animation \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Armature/datas \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Armature/display \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Armature/physics \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Armature/utils \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/GUI \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/GUI/BaseClasses \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/GUI/Layouts \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/GUI/System \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/GUI/UIWidgets \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Json \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Json/rapidjson \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Json/rapidjson/internal \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Reader \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Reader/WidgetReader \
	$(COCOS2DX_EXT_ROOT)/CocoStudio/Trigger
LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES) \
	$(LOCAL_PATH)/src \
	$(LOCAL_PATH)/src/platform/android \
	$(COCOS2DX_ROOT)/cocos2dx/support/tinyxml2
LOCAL_EXPORT_LDLIBS := -L$(LOCAL_PATH)/thirdparty/system_libs/$(TARGET_ARCH) -lsqlite -llog
LOCAL_LDLIBS := $(LOCAL_EXPORT_LDLIBS)
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static curl yajl aosp entities

include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,cocos2dx-better/thirdparty/aosp)
$(call import-module,cocos2dx-better/thirdparty/curl)
$(call import-module,cocos2dx-better/thirdparty/entities)
$(call import-module,cocos2dx-better/thirdparty/yajl)
