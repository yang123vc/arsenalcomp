APP_MODULES := libiconv
APP_PLATFORM := android-14

ifeq ($(NDK_DEBUG),1)
        APP_OPTIM := debug
else
        APP_OPTIM := release
endif