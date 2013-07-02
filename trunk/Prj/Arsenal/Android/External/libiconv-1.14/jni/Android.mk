LOCAL_PATH:= $(call my-dir)  
#libiconv.so
include $(CLEAR_VARS)  

TARGET_PLATFORM := android-14

LOCAL_MODULE := libiconv  
LOCAL_CFLAGS :=\
  -Wno-multichar\
  -DANDROID\
  -D_ANDROID\
  -DLIBDIR="c"\
  -DBUILDING_LIBICONV\
  -DIN_LIBRARY\
  
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
  
include $(BUILD_SHARED_LIBRARY)



