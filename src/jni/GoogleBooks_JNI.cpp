#include <condition_variable>
#include <jni.h>
#include <string>

#include "GoogleBooksService.h"
#include "../android/AndroidLogging.h"

static GoogleBooksService googleBooksService;

extern "C"
{
JNIEXPORT jstring JNICALL Java_com_example_booksclient_NativeApi_fetchBooks(JNIEnv *env, jclass clazz, const jstring query,
 const jint startIndex, const jint maxResults)
{
    std::string result;
    try
    {
        jboolean isCopy;
        const char* utf8 = env->GetStringUTFChars(query, &isCopy);
#ifdef ANDROID
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
                result = json;
#ifdef ANDROID
                LOGI("Fetched books: %s", result.c_str());
#endif
            }
        );
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

JNIEXPORT void JNICALL Java_com_example_booksclient_NativeApi_addToFavorites(JNIEnv *env, jclass clazz, const jstring bookId,
    const jstring bookJson)
{
    const char* cBookId = env->GetStringUTFChars(bookId, nullptr);
    const char* cBookJson = env->GetStringUTFChars(bookJson, nullptr);

    googleBooksService.AddToFavorites(cBookId, cBookJson);

    env->ReleaseStringUTFChars(bookId, cBookId);
    env->ReleaseStringUTFChars(bookJson, cBookJson);

#ifdef ANDROID
    LOGI("Add to favorites, BookId: %s", cBookId);
#endif
}

JNIEXPORT void JNICALL Java_com_example_booksclient_NativeApi_removeFromFavorites(JNIEnv *env, jclass clazz, const jstring bookId)
{
    const char* cBookId = env->GetStringUTFChars(bookId, nullptr);

    googleBooksService.RemoveFromFavorites(cBookId);

    env->ReleaseStringUTFChars(bookId, cBookId);
#ifdef ANDROID
    LOGI("Removed from favorites, BookId: %s", cBookId);
#endif
}

JNIEXPORT jboolean JNICALL Java_com_example_booksclient_NativeApi_isFavorite(JNIEnv *env, jclass clazz, const jstring bookId)
{
    const char* cBookId = env->GetStringUTFChars(bookId, nullptr);
    const bool isFav = googleBooksService.IsFavorite(cBookId);

#ifdef ANDROID
    LOGI("If book favorite: %hhd | id: %s | filePath: %s", isFav, cBookId, googleBooksService.GetFavoritesFilePath().c_str());
#endif

    env->ReleaseStringUTFChars(bookId, cBookId);
    return isFav ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL Java_com_example_booksclient_NativeApi_getFavoriteBooks(JNIEnv *env, jclass clazz)
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

JNIEXPORT void JNICALL Java_com_example_booksclient_NativeApi_setFavoritesFilePath(JNIEnv *env, jclass clazz, const jstring favoritesFilePath)
{
    const char* path = env->GetStringUTFChars(favoritesFilePath, nullptr);
    std::string favoritesPath(path);

    googleBooksService.SetFavoritesFilePath(favoritesPath);

    env->ReleaseStringUTFChars(favoritesFilePath, path);
#ifdef ANDROID
    LOGI("Set favorites path: %s", favoritesPath.c_str());
#endif
}

JNIEXPORT jstring JNICALL Java_com_example_booksclient_NativeApi_testNative(JNIEnv *env, jclass clazz)
{
    return env->NewStringUTF("Native test success!");
}
} // extern "C"
