LOCAL_PATH:= $(call my-dir)  
#Arsenal.so

include $(CLEAR_VARS)  



LOCAL_MODULE := Arsenal


LOCAL_CFLAGS :=\
        -Wno-multichar\
        -DANDROID\
        -D_ANDROID\
  
ifeq ($(APP_OPTIM),debug)
        LOCAL_CFLAGS := -O0 -DDEBUG -g $(LOCAL_CFLAGS)
else
        LOCAL_CFLAGS := -O2 -DNDEBUG -g $(LOCAL_CFLAGS)
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
        ../../../../Arsenal/Tengu/tengu.c\
                                                \
        ../../../../Arsenal/Tools/grammar_codegen.c\
        ../../../../Arsenal/Tools/grammar_collector.c\
        ../../../../Arsenal/Tools/iniobj.c\
        ../../../../Arsenal/Tools/serializer.c\
        ../../../../Arsenal/Tools/tools.c\
                                                \
        ../../../../Arsenal/Arsenal.c\
        


LOCAL_C_INCLUDES += \
        $(LOCAL_PATH)/../External/libiconv-1.14/jni/include\


LOCAL_LDLIBS := -l$(LOCAL_PATH)/../External/libiconv-1.14/libs/armeabi/libiconv.a



ifeq ($(LIB_MODE),shared)
        include $(BUILD_SHARED_LIBRARY)
else
        include $(BUILD_STATIC_LIBRARY)
endif






