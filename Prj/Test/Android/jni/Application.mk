NDK_TOOLCHAIN_VERSION=4.7
APP_PLATFORM := android-14

ifeq ($(NDK_DEBUG),1)
        APP_OPTIM := debug
else
        APP_OPTIM := release
endif



APP_MODULES := Test

