#ifndef VM_H
#define VM_H

#include <jni.h>
#include <dlfcn.h>

typedef int(*JNI_CreateJavaVM_t)(void *, void *, void *);
typedef jint(*registerNatives_t)(JNIEnv *env, jclass clazz);

int init_jvm(JavaVM **p_vm, JNIEnv **p_env);

#endif