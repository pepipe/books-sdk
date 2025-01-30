#include "AndroidStorageHelper.h"

std::string AndroidStorageHelper::GetInternalFilesDir(JNIEnv* env, jobject context) {
    jclass storageHelperClass = env->FindClass("com/example/booksclient/StorageHelper");
    if (storageHelperClass == nullptr) {
        return "";
    }

    jmethodID methodID = env->GetStaticMethodID(storageHelperClass, "getInternalFilesDir", "(Landroid/content/Context;)Ljava/lang/String;");
    if (methodID == nullptr) {
        return "";
    }

    jstring result = (jstring)env->CallStaticObjectMethod(storageHelperClass, methodID, context);
    const char* path = env->GetStringUTFChars(result, nullptr);
    std::string dir(path);

    env->ReleaseStringUTFChars(result, path);

    return dir;
}