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

jstring c_string_to_java(JNIEnv* env, const char* str);
jobject c_enum_to_java(JNIEnv* env, const char* clazz);
void set_enum_field(JNIEnv* env, jclass clazz, jobject obj,
        const char* field_name, const char* enum_name, jsize enum_value);
void set_int_field(JNIEnv* env, jclass clazz, jobject obj,
        const char* field_name, jint value);
void set_string_field(JNIEnv* env, jclass clazz, jobject obj,
        const char* field_name, const char* value);
