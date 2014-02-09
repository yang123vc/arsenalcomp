NDK_TOOLCHAIN_VERSION=4.8
APP_PLATFORM := android-18

ifeq ($(NDK_DEBUG),1)
        APP_OPTIM := debug
else
        APP_OPTIM := release
endif



APP_MODULES := libiconv Arsenal

APP_ABI := armeabi

ifeq ($(PLAT),armeabi)
        APP_ABI := armeabi
endif

ifeq ($(PLAT),armeabi-v7a)
        APP_ABI := armeabi-v7a
endif

ifeq ($(PLAT),mips)
        APP_ABI := mips
endif

ifeq ($(PLAT),x86)
        APP_ABI := x86
endif

ifeq ($(LIB_MODE),shared)
	bin_dir := dll
else
	bin_dir := lib
endif

NDK_APP_OUT := ../../../../Temp/$(APP_ABI)/$(APP_OPTIM)/$(bin_dir)
#NDK_APP_DST_DIR
