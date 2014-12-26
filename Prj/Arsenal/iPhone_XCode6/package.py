#coding utf-8

import sys;
import io;
import os;

import shutil;

import subprocess;
import plistlib;
import json;
import os;
import sys;
import traceback;
import tempfile;
import shutil;
import time;



def fixpathend(path):
        assert(isinstance(path, str));
        if len(path) == 0:
                return "";
        if path[len(path)-1] != "/":
                path = path + "/";
                pass;
        
        return path;



def copyfile_for_path_name(src, dest, name):
        assert(isinstance(src, str));
        assert(isinstance(dest, str));
        assert(isinstance(name, str));
        dest_path = dest;
        src = fixpathend(src) + name;
        dest = fixpathend(dest) + name;
        
        
        
        if os.path.exists(dest):
                if os.path.isfile(dest):
                        os.remove(dest);
                        pass;
                else:
                        shutil.rmtree(dest);
                        pass;
                
                pass;      
        
        if not os.path.exists(dest_path):
                os.makedirs(dest_path);
                pass;
        
        
        print(src);
        print(dest);
        shutil.copyfile(src, dest);
        return True;


def copyfile_for_path_name2(src, dest, src_name, dest_name):
        assert(isinstance(src, str));
        assert(isinstance(dest, str));
        assert(isinstance(src_name, str));
        assert(isinstance(dest_name, str));
        
        dest_path = dest;
        src = fixpathend(src) + src_name;
        dest = fixpathend(dest) + dest_name;
        
        
        
        if os.path.exists(dest):
                if os.path.isfile(dest):
                        os.remove(dest);
                        pass;
                else:
                        shutil.rmtree(dest);
                        pass;
                
                pass;      
        
        if not os.path.exists(dest_path):
                os.makedirs(dest_path);
                pass;
        
        
        print(src);
        print(dest);
        shutil.copyfile(src, dest);
        return True;



def generate_tmp_path(name):
        assert(isinstance(name, str));
        path = tempfile.gettempdir();
        if path[len(path) - 1] != "/":
                path = path + "/";
                pass;
        tmp_num = 0;
        while True:
                tmp_name = str(tmp_num) + name;
                if os.path.exists(path + tmp_name):
                        tmp_num = tmp_num + 1;
                        pass;
                else:
                        break;
                pass;
        return path + tmp_name;


def unzip_file(src_path, dest_dir):

        assert(isinstance(src_path, str));
        assert(isinstance(dest_dir, str));
        
        dest_dir = os.path.expanduser(dest_dir);
        src_path = os.path.expanduser(src_path);
        
        dest_dir = fixpathend(dest_dir);
        dest_dir = os.path.abspath(dest_dir);
        
        src_path = os.path.abspath(src_path);
        
        if not os.path.exists(src_path):
                print("not such file : " + src_path);
                return False;
        
        save_dir = os.path.abspath(".");
        
        dirname = os.path.dirname(src_path);
        basename = os.path.basename(src_path);
        
        unziped_name = os.path.splitext(basename)[0];
        unziped_path = dest_dir + "/" + unziped_name;
        dirname = fixpathend(dirname);
        
        print(dirname);
        print(basename);
        print(unziped_name);
        print(unziped_path);

        if os.path.exists(unziped_path):
                shutil.rmtree(unziped_path);
                pass;
        
        
        os.chdir(dirname);
        
           
        print("execute : 'unzip %s -d %s'" % (basename, dest_dir));

        subprocess.call(["unzip", basename, "-d", dest_dir]);   
        
        os.chdir(save_dir);
        
        if not os.path.exists(dest_dir):
                print(dest_dir +" not existed!");
                return False;
        
        
        return True;

        
        
def copy_to_package(codebase_root_path, binary_path, package_dir, product_app_name):
        assert(isinstance(codebase_root_path, str));
        assert(isinstance(binary_path, str));
        assert(isinstance(product_app_name, str));
        assert(isinstance(package_dir, str));
        
        codebase_root_path = fixpathend(codebase_root_path);
        binary_path = fixpathend(binary_path);
        
        
        try:
                src = binary_path + product_app_name + "/";
                dest_dir = codebase_root_path + "Package/" + package_dir + "/";
                dest = dest_dir + product_app_name + ".zip";
                temp_path = generate_tmp_path(product_app_name + ".zip");
                
                print(src);
                print(dest);
                print(temp_path);
                
                if not os.path.exists(src):
                        return False;
                
                if os.path.exists(dest):
                        os.remove(dest);
                        pass;
                
                
                """
                print("execute : 'zip -r %s %s'" % (temp_path, src));
                subprocess.call(["zip", "-r", temp_path, src]);
                """
                
                save_dir = os.path.abspath(".");
                os.chdir(binary_path);                
                
                print("execute : 'zip -r %s %s'" % (temp_path, product_app_name));
                subprocess.call(["zip", "-r", temp_path, product_app_name]);                
                
                os.chdir(save_dir);
                
                if not os.path.exists(temp_path):
                        print(temp_path +" not existed!");
                        return False;
                
                if not os.path.exists(dest_dir):
                        os.makedirs(dest_dir);
                        pass;
                
                shutil.copyfile(temp_path, dest);
                
                return True;
        except Exception as e:
                print(e);
                pass;
        
        return False;


##########################################################################################################


def pack_dylib(codebase_root_path, binary_path, package_dir, product_app_name, configuration):

        print(codebase_root_path);
        
        frameworks_path = codebase_root_path + package_dir;
        
        frameworks_path = fixpathend(frameworks_path);
        
        print(frameworks_path);
        print(product_app_name);
        
        copyfile_for_path_name(binary_path, frameworks_path, product_app_name);
    
        return True;


