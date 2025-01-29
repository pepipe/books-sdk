#pragma once

#ifdef ANDROID
#include <android/log.h>
#define LOG_TAG "NativeApi"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#endif
