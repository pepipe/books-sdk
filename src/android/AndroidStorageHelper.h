#pragma once

#include <jni.h>
#include <string>

class AndroidStorageHelper {
public:
    static std::string GetInternalFilesDir(JNIEnv* env, jobject context);
};