#include <jniw/jniw.h>

#include <string.h>
#include <stdlib.h>

jstring jniw_to_jstring(JNIEnv* env, const char* str)
{
    int len = strlen(str);
    jbyte* nativeStr = (jbyte*)(str);
    jbyteArray bytes = (*env)->NewByteArray(env, (jsize)(len));
    (*env)->SetByteArrayRegion(env, bytes, 0, len, nativeStr);

    jclass charsetClass = (*env)->FindClass(env, "java/nio/charset/Charset");
    jmethodID forName = (*env)->GetStaticMethodID(env, charsetClass,
            "forName", "(Ljava/lang/String;)Ljava/nio/charset/Charset;");
    jstring utf8 = (*env)->NewStringUTF(env, "UTF-8");
    jobject charset = (*env)->CallStaticObjectMethod(env, charsetClass,
            forName, utf8);

    jclass stringClass = (*env)->FindClass(env, "java/lang/String");
    jmethodID ctor = (*env)->GetMethodID(env, stringClass, "<init>",
            "([BLjava/nio/charset/Charset;)V");

    jstring ret = (jstring)((*env)->NewObject(env, stringClass,
                ctor, bytes, charset));

    (*env)->DeleteLocalRef(env, bytes);
    (*env)->DeleteLocalRef(env, charsetClass);
    (*env)->DeleteLocalRef(env, utf8);
    (*env)->DeleteLocalRef(env, charset);
    (*env)->DeleteLocalRef(env, stringClass);

    return ret;
}

jobject jniw_to_enum(JNIEnv* env, const char* clazz)
{
    const char* retStart = "()[";
    char* clazzName;
    int clazzLen = strlen(clazz);
    int retLen = strlen(retStart);

    if (clazz[0] == 'L' && clazz[clazzLen-1] == ';') {
        clazzName = malloc(clazzLen - 2);
        strncpy(clazzName, clazz + 1, clazzLen - 2);
        clazzName[clazzLen - 2] = '\0';
    } else {
        clazzName = malloc(clazzLen + 1);
        strncpy(clazzName, clazz, clazzLen);
        clazzName[clazzLen] = '\0';
    }

    char* ret = malloc(retLen + clazzLen + 1);
    ret[0] = '\0';
    strncat(ret, retStart, retLen);
    strncat(ret, clazz, clazzLen);

    jclass c = (*env)->FindClass(env, clazzName);
    jmethodID values = (*env)->GetStaticMethodID(env, c, "values", ret);

    free(ret);
    free(clazzName);
    return (*env)->CallStaticObjectMethod(env, c, values);
}

jobject jniw_arraylist_create_r(JNIEnv* env, jsize reserve)
{
    jclass arrayListClass = (*env)->FindClass(env, "java/util/ArrayList");
    jmethodID ctor = (*env)->GetMethodID(env, arrayListClass, "<init>",
            "(I)V");
    return (*env)->NewObject(env, arrayListClass, ctor, reserve);
}

void jniw_arraylist_add(JNIEnv* env, jobject arraylist, jobject to_add)
{
    jclass arrayListClass = (*env)->FindClass(env, "java/util/ArrayList");
    jmethodID addMethod = (*env)->GetMethodID(env, arrayListClass, "add",
            "(Ljava/lang/Object;)Z");
    (*env)->CallBooleanMethod(env, arraylist, addMethod, to_add);
    (*env)->DeleteLocalRef(env, to_add);
}

void jniw_set_enum_field(JNIEnv* env,
        jclass clazz,
        jobject obj,
        const char* field_name,
        const char* enum_name,
        jsize enum_value)
{
    jfieldID field = (*env)->GetFieldID(env, clazz, field_name, enum_name);
    jobjectArray objA = jniw_to_enum(env, enum_name);
    jobject o = (*env)->GetObjectArrayElement(env, objA, enum_value);
    (*env)->SetObjectField(env, obj, field, o);
    (*env)->DeleteLocalRef(env, o);
    (*env)->DeleteLocalRef(env, objA);
}

void jniw_set_int_field(JNIEnv* env, jclass clazz, jobject obj,
        const char* field_name, jint value)
{
    jfieldID field = (*env)->GetFieldID(env, clazz, field_name, "I");
    (*env)->SetIntField(env, obj, field, value);
}

void jniw_set_string_field(JNIEnv* env, jclass clazz, jobject obj,
        const char* field_name, const char* value)
{
    jfieldID field = (*env)->GetFieldID(env, clazz,
            field_name, "Ljava/lang/String;");
    jstring s = jniw_to_jstring(env, value);
    (*env)->SetObjectField(env, obj, field, s);
    (*env)->DeleteLocalRef(env, s);
}
