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


NDK_BUILD_CMD = "D:\\Tools\\Dev\\Android\\NDK\\r9b\\ndk-build.cmd";


def fixpathend(path):
        assert(isinstance(path, str));
        if len(path) == 0:
                return "";
        if path[len(path)-1] != "\\":
                path = path + "\\";
                pass;
        
        return path;


def call_ndk_build(is_debug, is_shared, plat_name, codebase, binary_output, temp_path, save_dir):
        
        assert(isinstance(plat_name, str));
        assert(isinstance(codebase, str));
        assert(isinstance(binary_output, str));
        assert(isinstance(temp_path, str));
        assert(isinstance(save_dir, str));
        
        
        dbg_str = "NDK_DEBUG=0";
        if is_debug:
                dbg_str = "NDK_DEBUG=1";
                pass;
        
        lib_str = "LIB_MODE=static";
        if is_shared:
                lib_str = "LIB_MODE=shared";
                pass;
        
        plat_str = "PLAT=" + plat_name;
        
        
        app_dst_dir_str = "NDK_APP_DST_DIR=";
        
        app_dst_dir = (codebase + "Binary\\");
        app_dst_dir += plat_name;
        app_dst_dir += "\\";
        
        
        
        if is_debug:
                app_dst_dir += "debug\\";
                pass;
        else:
                app_dst_dir += "release\\";
                pass;
        
        if is_shared:
                app_dst_dir += "dll\\";
                pass;
        else:
                app_dst_dir += "lib\\";
                pass;
        
        app_dst_dir_str += app_dst_dir;
        
        print(app_dst_dir_str);        


        try:
                if not os.path.exists(binary_output):
                        os.makedirs(binary_output);
                        pass;
                
                if os.path.exists(temp_path) and os.path.isdir(temp_path):
                        shutil.rmtree(temp_path);
                        pass;
                
                
                os.chdir(save_dir + "..\\jni\\");
                
                subprocess.call([NDK_BUILD_CMD, dbg_str, lib_str, plat_str, app_dst_dir_str]);
                
                os.chdir(save_dir);

                if is_shared:                   #build and copy is done for shared library
                        return True;
                else:
                        shutil.copyfile(temp_path + "libiconv.a", app_dst_dir + "libiconv.a");
                        shutil.copyfile(temp_path + "Arsenal.a", app_dst_dir + "Arsenal.a");
                        return True;
                
                return True;
        except Exception as e:
                print(e);
                ret = False;
                os.chdir(save_dir);
                pass;
        
        
        return False;
        
        
        

def build_arsenal(is_debug, is_shared, plat_name):
        assert(isinstance(plat_name, str));
        
        codebase_dir = os.path.abspath(".");
        codebase_dir = fixpathend(codebase_dir);
        codebase_dir +=  "..\\..\\..\\..\\";
        codebase_dir = os.path.abspath(codebase_dir);
        codebase_dir = fixpathend(codebase_dir);
        
        
        binary_path = codebase_dir + "Binary\\";
        temp_path = codebase_dir + "Temp\\";        
        
        temp_path += plat_name;
        temp_path += "\\";
        
        binary_path += plat_name;
        binary_path += "\\";
        
        if is_debug:
                temp_path += "debug\\";
                binary_path += "debug\\";
                pass;
        else:
                temp_path += "release\\";
                binary_path += "release\\";
                pass;
        
        if is_shared:
                temp_path += "dll\\";
                binary_path += "dll\\";
                pass;
        else:
                temp_path += "lib\\";
                binary_path += "lib\\";
                pass;
        
        temp_path += "local\\";
        temp_path += plat_name;
        temp_path += "\\";
        
        save_dir = os.path.abspath(".");
        save_dir = fixpathend(save_dir);                
        
        print(codebase_dir);
        print(save_dir);
        print(binary_path);
        print(temp_path);
        
  
        ret = call_ndk_build(is_debug, is_shared, plat_name, codebase_dir, binary_path, temp_path, save_dir);        
        
        return ret;





"""
def main():
        if not hasattr(sys, "argv"):
                return "False";
        
        print(sys.argv);
        
        
        is_debug = False;
        is_shared = False;
        plat_name = "armeabi";
        
        print(len(sys.argv));
        
        i = 1;
        
        while i < len(sys.argv):
                if sys.argv[i] == "debug":
                        is_debug = True;
                        pass;
                elif sys.argv[i] == "release":
                        is_debug = False;
                        pass;
                elif sys.argv[i] == "shared":
                        is_shared = True;
                        pass;
                elif sys.argv[i] == "static":
                        is_shared = False;
                        pass;
                elif sys.argv[i] == "arm":
                        plat_name = "armeabi";
                        pass;
                elif sys.argv[i] == "x86":
                        plat_name = "x86";
                        pass;
                elif sys.argv[i] == "armv7":
                        plat_name = "armeabi-v7a";
                        pass;
                elif sys.argv[i] == "mips":
                        plat_name = "mips";
                        pass;
                else:
                        print("invalid parameter : " + sys.argv[i]);
                        pass;
                
                
                
                i = i + 1;
                pass;
        
        
        
        if not build_external_libraries(plat_name):
                print("failed to build external libraries\r\n");
                sys.exit(-1);
                pass;
        
        
        if not build_arsenal(is_debug, is_shared, plat_name):
                print("failed to build Arsenal shared library\r\n");
                sys.exit(-1);
                pass;
        
        pass;
"""


platform_list = ["armeabi", "armeabi-v7a", "x86"];

for plat in platform_list:
        build_arsenal(True, False, plat);
        build_arsenal(False, False, plat);
        build_arsenal(True, True, plat);
        build_arsenal(False, True, plat);
        pass;




