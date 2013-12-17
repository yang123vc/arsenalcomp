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

def build_external_libraries(plat_name):
        
        assert(isinstance(plat_name, str));
        
        save_dir = os.path.abspath(".");
        save_dir = fixpathend(save_dir);
        print(save_dir);
        
        binary_path = save_dir + "../../../../Binary/";
        
        #iconv ########################
        iconv_path = save_dir + ".." + "/External/libiconv/1.14/";
        
        iconv_static_lib_path = iconv_path + "obj/local/" + plat_name + "/libiconv.a";
        iconv_shared_lib_path = iconv_path + "libs/" + plat_name + "/libiconv.so";
        
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
        
                plat_str = "PLAT=" + plat_name;
                
                os.chdir(iconv_path + "jni/");
                subprocess.call(["ndk-build", "NDK_DEBUG=0", "LIB_MODE=static", plat_str, "-B"]);
                os.chdir(save_dir);
                shutil.copyfile(iconv_static_lib_path, external_libs_path + "libiconv.a");


                os.chdir(iconv_path + "jni/");
                subprocess.call(["ndk-build", "NDK_DEBUG=0", "LIB_MODE=shared", plat_str, "-B"]);
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



def build_arsenal(is_debug, is_shared, plat_name):
        
        assert(isinstance(plat_name, str));
        
        save_dir = os.path.abspath(".");
        save_dir = fixpathend(save_dir);
        print(save_dir);
        
        binary_path = save_dir + "../../../../Binary/";
        
        
        if is_shared:
                binary_path += "dll/";
                if is_debug:
                    binary_path += "debug/";
                    pass;
                else:
                    binary_path += "release/";
                    pass;
                pass;
                
                arsenal_binary_file = binary_path + "Arsenal.so";
                pass;
        else:
                binary_path += "lib/";
                if is_debug:
                    binary_path += "debug/";
                    pass;
                else:
                    binary_path += "release/";
                    pass;
                pass;
                
                arsenal_binary_file = binary_path + "Arsenal.a";
                pass;
        
        
        print(binary_path);
        print(arsenal_binary_file);
        
        
        arsenal_prj_path = save_dir + "../";
        
        arsenal_temp_lib_path = arsenal_prj_path + "obj/local/" + plat_name + "/Arsenal.a";
        if is_shared:
                arsenal_temp_lib_path = arsenal_prj_path + "libs/" + plat_name + "/Arsenal.so";
                pass;
        
        print(arsenal_prj_path);
        print(arsenal_temp_lib_path);

        ret = True;
        
        try:
                if not os.path.exists(binary_path):
                        os.makedirs(binary_path);
                        pass;
                
                
                if os.path.exists(arsenal_binary_file):
                        if os.path.isfile(arsenal_binary_file):
                                os.remove(arsenal_binary_file);
                                pass;
                        pass;        
                
                
                dbg_str = "NDK_DEBUG=0";
                if is_debug:
                        dbg_str = "NDK_DEBUG=1";
                        pass;
                
                lib_str = "LIB_MODE=static";
                if is_shared:
                        lib_str = "LIB_MODE=shared";
                        pass;
                        
                plat_str = "PLAT=" + plat_name;
                
                
                os.chdir(arsenal_prj_path + "jni/");
                subprocess.call(["ndk-build", dbg_str, lib_str, plat_str, "-B"]);
                os.chdir(save_dir);
                shutil.copyfile(arsenal_temp_lib_path, arsenal_binary_file);
                
                
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

main();  #this function used in command-line mode


