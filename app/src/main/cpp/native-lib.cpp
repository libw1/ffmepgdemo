#include <jni.h>
#include <string>
#include<pthread.h>

extern "C" JNIEXPORT jstring

JNICALL
Java_conykais_ffmepgdemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C" JNIEXPORT jint

JNICALL
Java_conykais_ffmepgdemo_MainActivity_run(JNIEnv *env, jobject object,jobject context) {
    std::string aaaa = "Hello ni hao!";
    jstring p = env->NewStringUTF(aaaa.c_str());
    char* className = "conykais/ffmepgdemo/MainActivity";
    jclass clazz = env->FindClass(className);
    jmethodID method = env->GetMethodID(clazz,"callFromJNI","(Ljava/lang/String;)V");
    env->CallVoidMethod(object,method,p);

//    jclass tclss = env->FindClass("android/widget/Toast");
//    jmethodID mid = env->GetStaticMethodID(tclss,"makeText","(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
//    jobject job = env->CallStaticObjectMethod(tclss,mid,context,p);
//    jmethodID showId = env->GetMethodID(tclss,"show","()V");
//    env->CallVoidMethod(job,showId,context,p);
    return 12345;
}



