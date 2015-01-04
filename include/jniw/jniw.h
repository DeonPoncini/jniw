#include <jni.h>

#ifdef ANDROID
#include <android/log.h>
#else
#include <stdio.h>
#endif

#ifdef ANDROID
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define LOGE(...) printf(__VA_ARGS__)
#endif

#define JNI_RETURN(name) JNIEXPORT name JNICALL

jstring jniw_to_jstring(JNIEnv* env, const char* str);
jobject jniw_to_enum(JNIEnv* env, const char* clazz);
jobject jniw_arraylist_create_r(JNIEnv* env, jsize reserve);
void jniw_arraylist_add(JNIEnv* env, jobject arraylist, jobject to_add);

void jniw_set_enum_field(JNIEnv* env, jclass clazz, jobject obj,
        const char* field_name, const char* enum_name, jsize enum_value);
void jniw_set_int_field(JNIEnv* env, jclass clazz, jobject obj,
        const char* field_name, jint value);
void jniw_set_string_field(JNIEnv* env, jclass clazz, jobject obj,
        const char* field_name, const char* value);
