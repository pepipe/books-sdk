#include <jni.h>
#include <string>
#include <thread>

#include "GoogleBooksService.h"

static GoogleBooksService googleBooksService;

extern "C"
{
JNIEXPORT jstring JNICALL Java_com_example_booksclient_services_GoogleBooksService_fetchBooks
(JNIEnv *env, jclass clazz, const jstring query,
 const jint startIndex, const jint maxResults)
{
    std::string result;
    try
    {
        jboolean isCopy;
        const char *utf8 = env->GetStringUTFChars(query, &isCopy);
        std::string cQuery(utf8);
        env->ReleaseStringUTFChars(query, utf8);

        googleBooksService.FetchBooks(
            cQuery,
            startIndex,
            maxResults,
            [&](const std::string &json, int, const std::string &)
            {
                result = json;
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

JNIEXPORT void JNICALL Java_com_example_booksclient_services_GoogleBooksService_addToFavorites(
    JNIEnv *env, jclass clazz, const jstring bookId,
    const jstring bookJson)
{
    const char *cBookId = env->GetStringUTFChars(bookId, nullptr);
    const char *cBookJson = env->GetStringUTFChars(bookJson, nullptr);

    googleBooksService.AddToFavorites(cBookId, cBookJson);

    env->ReleaseStringUTFChars(bookId, cBookId);
    env->ReleaseStringUTFChars(bookJson, cBookJson);
}

JNIEXPORT jboolean JNICALL Java_com_example_booksclient_services_GoogleBooksService_isFavorite(
    JNIEnv *env, jclass clazz, const jstring bookId)
{
    const char *cBookId = env->GetStringUTFChars(bookId, nullptr);
    bool isFav = googleBooksService.IsFavorite(cBookId);
    env->ReleaseStringUTFChars(bookId, cBookId);
    return isFav ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL Java_com_example_booksclient_services_GoogleBooksService_getFavoriteBooks(
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
