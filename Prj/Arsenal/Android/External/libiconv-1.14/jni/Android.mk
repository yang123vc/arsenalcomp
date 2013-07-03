LOCAL_PATH:= $(call my-dir)  

include $(CLEAR_VARS)  


LOCAL_MODULE := libiconv  
LOCAL_CFLAGS :=\
  -Wno-multichar\
  -DANDROID\
  -D_ANDROID\
  -DLIBDIR="c"\
  -DBUILDING_LIBICONV\
  -DIN_LIBRARY\
  

ifeq ($(NDK_DEBUG),1)
        LOCAL_CFLAGS := -O0 -DDEBUG -g $(LOCAL_CFLAGS)
else
        LOCAL_CFLAGS := -O2 -DNDEBUG -g $(LOCAL_CFLAGS)
endif
  


LOCAL_SRC_FILES := \
    libcharset/lib/localcharset.c\
    lib/iconv.c \
    lib/relocatable.c\
  
LOCAL_C_INCLUDES += \
  $(LOCAL_PATH)/include\
  $(LOCAL_PATH)/libcharset\
  $(LOCAL_PATH)/lib \
  $(LOCAL_PATH)/libcharset/include \
  $(LOCAL_PATH)/srclib \
  
TARGET_PLATFORM := android-14


ifeq ($(LIB_MODE),shared)
        include $(BUILD_SHARED_LIBRARY)
else
        include $(BUILD_STATIC_LIBRARY)
endif






