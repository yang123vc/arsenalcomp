# coding utf-8

import sys;
import io;
import os;
import shutil;
import subprocess;
import plistlib;
import json;
import traceback;
import tempfile;
import time;
import signal;
import re;



def fixpathend(path):
        assert(isinstance(path, str));
        if len(path) == 0:
                return "";
        if path[len(path)-1] != "/":
                path = path + "/";
                pass;
        
        return path;






#build iconv and copy it to binary

def build_external_libraries():
        
        save_dir = os.path.abspath(".");
        save_dir = fixpathend(save_dir);
        print(save_dir);
        binary_path = save_dir + "../../../../Binary/";
        
        #iconv ########################
        iconv_path = save_dir + ".." + "/External/libiconv-1.14/";
        
        iconv_static_lib_path = iconv_path + "obj/local/armeabi/libiconv.a";
        iconv_shared_lib_path = iconv_path + "libs/armeabi/libiconv.so";
        
        print(iconv_path);
        print(iconv_static_lib_path);
        print(iconv_shared_lib_path);        

        
        external_libs_path = binary_path + "External/libs/";
        
        ret = True;
        
        try:        
                if os.path.exists(external_libs_path):
                        if os.path.isdir(external_libs_path):
                                shutil.rmtree(external_libs_path);
                                pass;
                        pass;        
        
                os.makedirs(external_libs_path);
        

                
                os.chdir(iconv_path + "jni/");
                subprocess.call(["ndk-build", "NDK_DEBUG=0", "LIB_MODE=static", "-B"]);
                os.chdir(save_dir);
                shutil.copyfile(iconv_static_lib_path, external_libs_path + "libiconv.a");


                os.chdir(iconv_path + "jni/");
                subprocess.call(["ndk-build", "NDK_DEBUG=0", "LIB_MODE=shared", "-B"]);
                os.chdir(save_dir);
                shutil.copyfile(iconv_shared_lib_path, external_libs_path + "libiconv.so");


                pass;
        except Exception as e:
                print(e);
                ret = False;
                pass;
        
        
        
        if os.path.exists(iconv_path + "libs"):
                if os.path.isdir(iconv_path + "libs"):
                        shutil.rmtree(iconv_path + "libs");
                        pass;
                pass;        


        if os.path.exists(iconv_path + "obj"):
                if os.path.isdir(iconv_path + "obj"):
                        shutil.rmtree(iconv_path + "obj");
                        pass;
                pass;      


        print(os.path.abspath("."));
        
        return ret;


def build_arsenal(is_debug):
        save_dir = os.path.abspath(".");
        save_dir = fixpathend(save_dir);
        print(save_dir);
        
        binary_path = save_dir + "../../../../Binary/";
        
        
        arsenal_static_binary_file = binary_path + "Arsenal.a";
        arsenal_shared_binary_file = binary_path + "Arsenal.so";
        
        
        arsenal_prj_path = save_dir + "../";
        
        arsenal_static_lib_path = arsenal_prj_path + "obj/local/armeabi/Arsenal.a";
        arsenal_shared_lib_path = arsenal_prj_path + "libs/armeabi/Arsenal.so";
        
        print(arsenal_prj_path);
        print(arsenal_static_lib_path);
        print(arsenal_shared_lib_path);

        ret = True;
        
        try:
        
                if os.path.exists(arsenal_static_binary_file):
                        if os.path.isfile(arsenal_static_binary_file):
                                os.remove(arsenal_static_binary_file);
                                pass;
                        pass;        
                
                if os.path.exists(arsenal_shared_binary_file):
                        if os.path.isfile(arsenal_shared_binary_file):
                                os.remove(arsenal_shared_binary_file);
                                pass;
                        pass;           
        
        

        
                dbg_str = "NDK_DEBUG=0";
                if is_debug:
                        dbg_str = "NDK_DEBUG=1";
                        pass;
                
                os.chdir(arsenal_prj_path + "jni/");
                subprocess.call(["ndk-build", dbg_str, "LIB_MODE=static", "-B"]);
                os.chdir(save_dir);
                shutil.copyfile(arsenal_static_lib_path, arsenal_static_binary_file);
                
                
                os.chdir(arsenal_prj_path + "jni/");
                subprocess.call(["ndk-build", dbg_str, "LIB_MODE=shared", "-B"]);
                os.chdir(save_dir);
                shutil.copyfile(arsenal_shared_lib_path, arsenal_shared_binary_file);
                
                pass;
        except Exception as e:
                print(e);
                ret = False;
                pass;
        
        
        if os.path.exists(arsenal_prj_path + "libs"):
                if os.path.isdir(arsenal_prj_path + "libs"):
                        shutil.rmtree(arsenal_prj_path + "libs");
                        pass;
                pass;        


        if os.path.exists(arsenal_prj_path + "obj"):
                if os.path.isdir(arsenal_prj_path + "obj"):
                        shutil.rmtree(arsenal_prj_path + "obj");
                        pass;
                pass;      


        print(os.path.abspath("."));
        
        return ret;

        pass;

        
        


if not build_external_libraries():
        print("failed to build external libraries\r\n");
        sys.exit(-1);
        pass;


if not build_arsenal(False):
        print("failed to build Arsenal library\r\n");
        sys.exit(-1);
        pass;


