#include <jni.h>
#include <string>
#include "GoogleBooksService.h"

static GoogleBooksService googleBooksService;

extern "C"
{
JNIEXPORT void JNICALL Java_BooksSDK_initialize(JNIEnv *env, jclass clazz)
{
}

JNIEXPORT jstring JNICALL Java_BooksSDK_fetchBooks(JNIEnv *env, jclass clazz, const jstring query,
                                                   const jint startIndex, const jint maxResults)
{
    const char *cQuery = env->GetStringUTFChars(query, nullptr);

    std::string result;
    try
    {
        googleBooksService.FetchBooks(
            cQuery,
            startIndex,
            maxResults,
            [&](const std::string& json, int, const std::string&)
            {
                result = json;
            }
        );
    } catch (const std::exception &e)
    {
        env->ReleaseStringUTFChars(query, cQuery);
        return env->NewStringUTF(e.what());
    }

    env->ReleaseStringUTFChars(query, cQuery);
    return env->NewStringUTF(result.c_str());
}

JNIEXPORT void JNICALL Java_BooksSDK_addToFavorites(JNIEnv *env, jclass clazz, const jstring bookId,
                                                    const jstring bookJson)
{
    const char *cBookId = env->GetStringUTFChars(bookId, nullptr);
    const char *cBookJson = env->GetStringUTFChars(bookJson, nullptr);

    googleBooksService.AddToFavorites(cBookId, cBookJson);

    env->ReleaseStringUTFChars(bookId, cBookId);
    env->ReleaseStringUTFChars(bookJson, cBookJson);
}

JNIEXPORT jboolean JNICALL Java_BooksSDK_isFavorite(JNIEnv *env, jclass clazz, const jstring bookId)
{
    const char *cBookId = env->GetStringUTFChars(bookId, nullptr);
    bool isFav = googleBooksService.IsFavorite(cBookId);
    env->ReleaseStringUTFChars(bookId, cBookId);
    return isFav ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL Java_BooksSDK_getFavoriteBooks(JNIEnv *env, jclass clazz)
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
