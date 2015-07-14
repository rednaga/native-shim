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

#include <dlfcn.h>
#include <unistd.h> // access

int main(int argc, const char* argv[])
{
  printf("[*] native-shim - diff");

  printf(" [+] Attempting to load : [ %s ]\n", argv[1]);

  if(access(argv[1], F_OK) == -1 ) {
    printf(" [!] File does not exist!\n");
    return -1;
  }

  // Should call initializers that are not JNI_OnLoad (ex. init_array)
  void* handle = dlopen(argv[1], RTLD_LAZY);

  if(handle == NULL) {
    printf(" [!] Could not dlopen file! Check permissions.\n");
    return -1;
  }

  printf(" [+] Library Loaded!");

  // TODO : Call onLoad functionality
  // Get function call
  void* vonLoad = dlsym(handle, "JNI_OnLoad");

  /*
  void *myso = dlopen("/path/to/my.so", RTLD_NOW);
  some_func *func = dlsym(myso, "function_name_to_fetch");
  func("foo");
  dlclose(myso);
  */
}

