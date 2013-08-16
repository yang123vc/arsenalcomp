APP_MODULES := libiconv
APP_PLATFORM := android-14

ifeq ($(NDK_DEBUG),1)
        APP_OPTIM := debug
else
        APP_OPTIM := release
endif

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




