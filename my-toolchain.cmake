#
# Allows compiling outside of Android-Studio.
#

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_TOOLCHAIN_DIRECTORY "$ENV{ANDROID_NDK_HOME}/build/cmake")
if(NOT CMAKE_TOOLCHAIN_FILE)
    if(NOT ANDROID_ABI)
        set(ANDROID_ABI "arm64-v8a")
    endif()
    if(NOT ANDROID_PLATFORM)
        set(ANDROID_PLATFORM "android-21")
    endif()
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_TOOLCHAIN_DIRECTORY}/android.toolchain.cmake")
endif()

if(WIN32 AND NOT CMAKE_MAKE_PROGRAM)
    set(CMAKE_MAKE_PROGRAM "$ENV{ANDROID_HOME}/ndk-bundle/prebuilt/windows/bin/make.exe" CACHE INTERNAL "" FORCE)
endif()