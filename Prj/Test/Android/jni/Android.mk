
LOCAL_PATH:= $(call my-dir)


#Test
include $(CLEAR_VARS)  

LOCAL_MODULE := Test
LOCAL_MODULE_FILENAME := Test

LOCAL_CFLAGS :=\
        -Wno-multichar\
        -DANDROID\
        -D_ANDROID\
  
ifeq ($(NDK_DEBUG),1)
        LOCAL_CFLAGS := -DDEBUG $(LOCAL_CFLAGS)
else
        LOCAL_CFLAGS := -DNDEBUG $(LOCAL_CFLAGS)
endif


LOCAL_SRC_FILES := ../Test/main.c



LOCAL_C_INCLUDES += \
        ../../../../Arsenal/    \





ifeq ($(LIB_MODE),shared)
        LOCAL_LDLIBS := -l$(LOCAL_PATH)/../../../../Binary/armeabi/release/dll/Arsenal.so
else
        LOCAL_LDLIBS := -l$(LOCAL_PATH)/../../../../Binary/armeabi/release/lib/Arsenal.a
endif


include $(BUILD_EXECUTABLE)
