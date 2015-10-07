LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
  shim.c
LOCAL_C_INCLUDE := ${ANDROID_NDK_ROOT}/platforms/android-21/arch-arm/usr/include/

LOCAL_MODULE := shim
LOCAL_MODULE_TAGS := optional

# Allow execution on android-16
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE

APP_ABI := armeabi armeabi-v7a x86

include $(BUILD_STATIC_EXECUTABLE)

include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
