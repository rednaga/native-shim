#include "vm.h"

int init_jvm(JavaVM **p_vm, JNIEnv **p_env) {
  //https://android.googlesource.com/platform/frameworks/native/+/ce3a0a5/services/surfaceflinger/DdmConnection.cpp
  JavaVMOption opt[4];
  opt[0].optionString = "-Djava.class.path=/data/local/tmp/target-app.apk";
  opt[1].optionString = "-agentlib:jdwp=transport=dt_android_adb,suspend=n,server=y";
  opt[2].optionString = "-Djava.library.path=/data/local/tmp";
  opt[3].optionString = "-verbose:jni"; // may want to remove this, it's noisy

  // Add this option if you're hacking stuff and need it, not normally required
  // opt[4].optionString = "-Xno-sig-chain"; // may not be require prior to ART vm, may even cause issues for DVM

  JavaVMInitArgs args;
  args.version = JNI_VERSION_1_6;
  args.options = opt;
  args.nOptions = 4; // Uptick this to 5, it will pass in the no-sig-chain option
  args.ignoreUnrecognized = JNI_FALSE;

  void *libdvm_dso = dlopen("libdvm.so", RTLD_NOW);
  void *libandroid_runtime_dso = dlopen("libandroid_runtime.so", RTLD_NOW);

  if (!libdvm_dso) {
    libdvm_dso = dlopen("libart.so", RTLD_NOW);
  }

  if (!libdvm_dso || !libandroid_runtime_dso) {
    return -1;
  }

  JNI_CreateJavaVM_t JNI_CreateJavaVM;
  JNI_CreateJavaVM = (JNI_CreateJavaVM_t) dlsym(libdvm_dso, "JNI_CreateJavaVM");
  if (!JNI_CreateJavaVM) {
    return -2;
  }

  registerNatives_t registerNatives;
  registerNatives = (registerNatives_t) dlsym(libandroid_runtime_dso, "Java_com_android_internal_util_WithFramework_registerNatives");
  if (!registerNatives) {
    // Attempt non-legacy version
    registerNatives = (registerNatives_t) dlsym(libandroid_runtime_dso, "registerFrameworkNatives");
    if(!registerNatives) {
      return -3;
    }
  }

  if (JNI_CreateJavaVM(&(*p_vm), &(*p_env), &args)) {
    return -4;
  }

  if (registerNatives(*p_env, 0)) {
    return -5;
  }

  return 0;
}

JNIEXPORT void InitializeSignalChain() {

}

JNIEXPORT void ClaimSignalChain() {

}

JNIEXPORT void UnclaimSignalChain() {

}

JNIEXPORT void InvokeUserSignalHandler() {

}

JNIEXPORT void EnsureFrontOfChain() {

}
