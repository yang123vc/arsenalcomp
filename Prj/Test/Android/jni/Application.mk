NDK_TOOLCHAIN_VERSION=4.8
APP_PLATFORM := android-18

ifeq ($(NDK_DEBUG),1)
        APP_OPTIM := debug
else
        APP_OPTIM := release
endif



APP_MODULES := Test

