LOCAL_PATH:= $(call my-dir)  
#Arsenal.so

include $(CLEAR_VARS)  



LOCAL_MODULE := Arsenal  
LOCAL_CFLAGS :=\
  -Wno-multichar\
  -DANDROID\
  -D_ANDROID\
  -DLIBDIR="c"\
  -DIN_LIBRARY\
  

LOCAL_SRC_FILES := \
        ../../../../Arsenal/Common/thread.c\
        ../../../../Arsenal/Common/utilities.c\
        ../../../../Arsenal/Common/textfile.c\
        ../../../../Arsenal/Common/strtable.c\
        ../../../../Arsenal/Common/string.c\
        ../../../../Arsenal/Common/random.c\
        ../../../../Arsenal/Common/vector.c\
        ../../../../Arsenal/Common/matrix.c\
        ../../../../Arsenal/Common/math.c\
        ../../../../Arsenal/Common/heap.c\
        ../../../../Arsenal/Common/crtstr.c\
        ../../../../Arsenal/Common/byteorder.c\
        ../../../../Arsenal/Common/base64.c\
        ../../../../Arsenal/Common/algo.c\
        ../../../../Arsenal/Common/filesystem.c\
       ../../../../Arsenal/Common/datastructure.c\
       ../../../../Arsenal/Common/enviroment.c\
       ../../../../Arsenal/Common/uri.c\
       ../../../../Arsenal/Common/strnum.c\
        ../../../../Arsenal/Common/memory.c\
       ../../../../Arsenal/Common/common.c\
        ../../../../Arsenal/Common/printf.c\
        ../../../../Arsenal/Common/strconv.c\
        ../../../../Arsenal/Arsenal.c\

  
LOCAL_C_INCLUDES += \
  $(LOCAL_PATH)/../External/libiconv-1.14/jni/include\
  
  
include $(BUILD_SHARED_LIBRARY)



