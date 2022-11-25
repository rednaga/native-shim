/*
 * Native 'shim' to assist in reversing JNI code
 *
 * Tim 'diff' Strazzere
 *   <diff@protonmail.com>
 *
 * Debug this application with whatever debugger
 * you'd prefer, pass the argument for which
 * library to load then just debug as normal.
 *
 * This should cause the "normal" initializers to
 * get hit.
 *
 */

#include <stdlib.h> // avoid exit warning
#include <stdio.h>
#include <string.h> // strstr

#include <dlfcn.h>  // dlopen/dlclose
#include <unistd.h> // access
#include <jni.h>    // jni stuff

#include "vm.h"

#if __aarch64__
#define APEX_LIBRARY_PATH "/apex/com.android.runtime/lib64/"
#else
#define APEX_LIBRARY_PATH "/apex/com.android.runtime/lib/"
#endif

typedef int (*JNI_OnLoadFunc)(void* vm, void* reserved);

int main(int argc, char *const argv[]) {
  printf("[*] native-shim - diff\n");
  printf(" [+] Attempting to load : [ %s ]\n", argv[1]);

  if(access(argv[1], F_OK) == -1 ) {
    printf(" [!] File does not exist!\n");
    return -1;
  }

  char* path = getenv("LD_LIBRARY_PATH");
  if(path == NULL || strstr(path, "apex") == NULL) {
    if(setenv("LD_LIBRARY_PATH", APEX_LIBRARY_PATH, 1) != 0) {
      printf(" [!] Error setting LD_LIBRARY_PATH via setenv!");
      return -1;
    } else {
      printf(" [+] Success setting env - reloading shim\n");
      int rc = execv( "/proc/self/exe", argv);
    }
  }

  // Should call initializers that are not JNI_OnLoad (ex. init_array)
  void* handle = dlopen(argv[1], RTLD_LAZY);

  if(handle == NULL) {
    printf(" [!] Could not dlopen file! (%s)\n", dlerror());
    return -1;
  }

  printf(" [+] Library Loaded!\n");

  // Get function call
  JNI_OnLoadFunc onLoadFunc = dlsym(handle, "JNI_OnLoad");
  if(onLoadFunc == NULL) {
    printf(" [!] No JNI_OnLoad found!\n");
    return -1;
  }

  printf(" [+] Initializing JavaVM Instance\n");
  JavaVM *vm = NULL;
  JNIEnv *env = NULL;
  int status = init_jvm(&vm, &env);
  if (status == 0) {
    printf(" [+] Initialization success (vm=%p, env=%p)\n", vm, env);
  } else {
    printf(" [!] Initialization failure (%i: %s)\n", status, dlerror());
    return -1;
  }

  printf(" [+] Found JNI_OnLoad, attempting to call\n");
  // Depending on the target you likely want to not pass null
  onLoadFunc(vm, NULL);

  printf(" [+] Closing library\n");
  dlclose(handle);

  return 0;
}
