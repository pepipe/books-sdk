#include <condition_variable>
#include <jni.h>
#include <mutex>
#include <string>
#include <thread>

#include "GoogleBooksService.h"

#ifdef BUILD_FOR_ANDROID
#include <android/log.h>
#define LOG_TAG "NativeApi"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#endif

static GoogleBooksService googleBooksService;

extern "C"
{
JNIEXPORT jstring JNICALL Java_com_example_booksclient_NativeApi_fetchBooks
(JNIEnv *env, jclass clazz, const jstring query,
 const jint startIndex, const jint maxResults)
{
    std::string result;
    bool done = false;  // Flag to signal when the callback completes
    std::mutex mtx;
    std::condition_variable cv;

    try
    {
        jboolean isCopy;
        const char *utf8 = env->GetStringUTFChars(query, &isCopy);
#ifdef BUILD_FOR_ANDROID
        LOGI("Fetching books with query: %s, startIndex: %d, maxResults: %d", utf8, startIndex, maxResults);
#endif
        std::string cQuery(utf8);
        env->ReleaseStringUTFChars(query, utf8);

        googleBooksService.FetchBooks(
            cQuery,
            startIndex,
            maxResults,
            [&](const std::string &json, int, const std::string &)
            {
                std::lock_guard lock(mtx);
                result = json;
                done = true;
#ifdef BUILD_FOR_ANDROID
                LOGI("Fetched books: %s", result.c_str());
#endif
                cv.notify_one();
            }
        );

        std::unique_lock lock(mtx);
        cv.wait(lock, [&]()
        {
            return done;
        });
    } catch (const std::exception &e)
    {
        return env->NewStringUTF(e.what());
    }

    // Create new UTF-16 java string from UTF-8 c++ string
    jbyteArray bytes = env->NewByteArray(result.length());
    env->SetByteArrayRegion(bytes, 0, result.length(), reinterpret_cast<const jbyte *>(result.c_str()));
    jstring encoding = env->NewStringUTF("UTF-8");
    jclass classString = env->FindClass("java/lang/String");
    jmethodID ctorString = env->GetMethodID(classString, "<init>", "([BLjava/lang/String;)V");
    jstring output = static_cast<jstring>(env->NewObject(classString, ctorString, bytes, encoding));

    env->DeleteLocalRef(bytes);
    env->DeleteLocalRef(encoding);
    env->DeleteLocalRef(classString);

    return output;
}

JNIEXPORT void JNICALL Java_com_example_booksclient_NativeApi_addToFavorites(
    JNIEnv *env, jclass clazz, const jstring bookId,
    const jstring bookJson)
{
    const char *cBookId = env->GetStringUTFChars(bookId, nullptr);
    const char *cBookJson = env->GetStringUTFChars(bookJson, nullptr);

    googleBooksService.AddToFavorites(cBookId, cBookJson);

    env->ReleaseStringUTFChars(bookId, cBookId);
    env->ReleaseStringUTFChars(bookJson, cBookJson);
}

JNIEXPORT void JNICALL Java_com_example_booksclient_NativeApi_removeFromFavorites(
JNIEnv *env, jclass clazz, const jstring bookId)
{
    const char *cBookId = env->GetStringUTFChars(bookId, nullptr);

    googleBooksService.RemoveFromFavorites(cBookId);

    env->ReleaseStringUTFChars(bookId, cBookId);
}

JNIEXPORT jboolean JNICALL Java_com_example_booksclient_NativeApi_isFavorite(
    JNIEnv *env, jclass clazz, const jstring bookId)
{
    const char *cBookId = env->GetStringUTFChars(bookId, nullptr);
    bool isFav = googleBooksService.IsFavorite(cBookId);
    env->ReleaseStringUTFChars(bookId, cBookId);
    return isFav ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL Java_com_example_booksclient_NativeApi_getFavoriteBooks(
    JNIEnv *env, jclass clazz)
{
    auto favorites = googleBooksService.GetFavoriteBooks();
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID ctor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jobject arrayList = env->NewObject(arrayListClass, ctor);

    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    for (const auto &book: favorites)
    {
        jstring bookStr = env->NewStringUTF(book.c_str());
        env->CallBooleanMethod(arrayList, addMethod, bookStr);
        env->DeleteLocalRef(bookStr);
    }
    return arrayList;
}
} // extern "C"
