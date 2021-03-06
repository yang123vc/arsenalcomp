
LOCAL_PATH:= $(call my-dir)


#Arsenal.so

include $(CLEAR_VARS)  



LOCAL_MODULE := Arsenal
LOCAL_MODULE_FILENAME := Arsenal

LOCAL_CFLAGS :=\
        -Wno-multichar\
        -DANDROID\
        -D_ANDROID\
  
ifeq ($(NDK_DEBUG),1)
        LOCAL_CFLAGS := -DDEBUG $(LOCAL_CFLAGS)
else
        LOCAL_CFLAGS := -DNDEBUG $(LOCAL_CFLAGS)
endif
  

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
        ../../../../Arsenal/Common/buffer.c\
                                                \
        ../../../../Arsenal/Lex/lex.c\
        ../../../../Arsenal/Lex/lex_misc.c\
        ../../../../Arsenal/Lex/match.c\
        ../../../../Arsenal/Lex/rgx_compile.c\
        ../../../../Arsenal/Lex/rgx_exec.c\
        ../../../../Arsenal/Lex/rgx_misc.c\
        ../../../../Arsenal/Lex/rgx_node.c\
        ../../../../Arsenal/Lex/rgx_parser.c\
                                            \
        ../../../../Arsenal/Parser/grammar.c\
        ../../../../Arsenal/Parser/lalr_build.c\
        ../../../../Arsenal/Parser/lalr_config.c\
        ../../../../Arsenal/Parser/lalr_state.c\
        ../../../../Arsenal/Parser/lr_action.c\
        ../../../../Arsenal/Parser/parser.c\
        ../../../../Arsenal/Parser/parser_in.c\
        ../../../../Arsenal/Parser/report.c\
        ../../../../Arsenal/Parser/symbol.c\
                                                \
        ../../../../Arsenal/Tools/grammar_codegen.c\
        ../../../../Arsenal/Tools/grammar_collector.c\
        ../../../../Arsenal/Tools/iniobj.c\
        ../../../../Arsenal/Tools/serializer.c\
        ../../../../Arsenal/Tools/property_list_internal.c\
        ../../../../Arsenal/Tools/property_list.c\
        ../../../../Arsenal/Tools/json.c\
        ../../../../Arsenal/Tools/tools.c	\
                                                \
        ../../../../Arsenal/Tengu/tengu.c	\
						\
        ../../../../Arsenal/Arsenal.c\
        


LOCAL_C_INCLUDES += \
        $(LOCAL_PATH)/../External/libiconv/1.14/include \


#LOCAL_LDLIBS := -l$(LOCAL_PATH)/../External/libiconv-1.14/libs/armeabi/libiconv.a
LOCAL_LDLIBS := -l$(LOCAL_PATH)/$(NDK_APP_OUT)/local/$(APP_ABI)/libiconv.a




ifeq ($(LIB_MODE),shared)
#        LOCAL_SHARED_LIBRARIES := libiconv
else
#        LOCAL_STATIC_LIBRARIES := libiconv
endif


ifeq ($(LIB_MODE),shared)
        include $(BUILD_SHARED_LIBRARY)
else
        include $(BUILD_STATIC_LIBRARY)
endif






