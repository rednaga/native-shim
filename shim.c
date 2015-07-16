/*
 * Native 'shim' to assist in reversing JNI code
 *
 * Tim 'diff' Strazzere
 *   <strazz@gmail.com>
 *   <diff@lookout.com>
 *
 * Debug this applicaiton with whatever debugger
 * you'd prefer, pass the argument for which
 * library to load then just debug as normal.
 *
 * This should cause the "normal" initializers to
 * get hit.
 * 
 */

#include <stdlib.h> // avoid exit warning
#include <stdio.h>

#include <dlfcn.h>  // dlopen/dlclose
#include <unistd.h> // access
#include <jni.h>    // jni stuff

// TODO : Mock a vm context
typedef int (*JNI_OnLoadFunc)(void* vm, void* reserved);
int (*onLoadFunc)(void) = NULL;

int main(int argc, const char* argv[]) {
  printf("[*] native-shim - diff\n");

  printf(" [+] Attempting to load : [ %s ]\n", argv[1]);

  if(access(argv[1], F_OK) == -1 ) {
    printf(" [!] File does not exist!\n");
    return -1;
  }

  // Should call initializers that are not JNI_OnLoad (ex. init_array)
  void* handle = dlopen(argv[1], RTLD_LAZY);

  if(handle == NULL) {
    char *errstr = dlerror();
    printf(" [!] Could not dlopen file! (%s)\n", errstr);
    return -1;
  }

  printf(" [+] Library Loaded!\n");

  // TODO : Call onLoad functionality
  // Get function call
  JNI_OnLoadFunc onLoad = dlsym(handle, "JNI_OnLoad");
  if(onLoad == NULL) {
    printf(" [!] No JNI_OnLoad found!\n");
    return -1;
  }

  onLoadFunc = onLoad;

  printf(" [+] Found JNI_OnLoad, attempting to call\n");
  onLoadFunc();

  printf(" [+] Closing library\n");
  dlclose(handle);

  return 0;
}

