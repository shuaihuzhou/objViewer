

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "../android_asset_operations.h"
#include "Application3D.h"

Application3D app3d;
JNIEnv *jniEnv;

extern "C" {
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_init(JNIEnv * env, jclass c,  jobject assetManager);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_loadObjModel(JNIEnv * env, jclass c, jstring filename, jboolean quickLoad);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setRenderBufferSize(JNIEnv * env, jclass c, jint w, jint h);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_frame(JNIEnv * env, jclass c);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_reset(JNIEnv * env, jclass c);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setDocDirectory(JNIEnv * env, jclass c, jstring docDir);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_rotate(JNIEnv * env, jclass c, jfloat deltaX, jfloat deltaY);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_translate(JNIEnv * env, jclass c, jfloat deltaX, jfloat deltaY);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_scale(JNIEnv * env, jclass c, jfloat s);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setBackgroundColor(JNIEnv * env, jclass c, jfloat r, jfloat g, jfloat b, jfloat a);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setModelColor(JNIEnv * env, jclass c, jfloat r, jfloat g, jfloat b, jfloat a);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setCameraPosition(JNIEnv * env, jclass c, jfloat x, jfloat y, jfloat z);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setLightPosition(JNIEnv * env, jclass c, jfloat x, jfloat y, jfloat z);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setLigthDirection(JNIEnv * env, jclass c, jfloat x, jfloat y, jfloat z);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setAmbientColor(JNIEnv * env, jclass c, jboolean r, jboolean g, jboolean b);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setDiffuseColor(JNIEnv * env, jclass c, jboolean r, jboolean g, jboolean b);
    JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_enableTexture(JNIEnv * env, jclass c, jboolean flag);

};

char* jstringTostring(JNIEnv* env, jstring jstr)
{        
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0)
    {
        rtn = (char*)malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->DeleteLocalRef(strencode);
    env->ReleaseByteArrayElements(barr, ba, 0);

    return rtn;
}

/////

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_init(JNIEnv * env, jclass c,jobject assetManager)
{
    app3d.init();

    // load shader from assets Folder
    AAssetManager *asset_manager = AAssetManager_fromJava(env, assetManager);
    FILE *fd;
    fd = android_fopen("standard.vert", "r", asset_manager);
    std::string gVertexShader = readToString(fd);
    fclose(fd);
    fd = android_fopen("directionalLight.frag", "r", asset_manager);
    std::string gFragmentShader = readToString(fd);
    fclose(fd);

    app3d.createShaders(gVertexShader.c_str(), gFragmentShader.c_str());
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_loadObjModel(JNIEnv * env, jclass c, jstring filename, jboolean quickLoad)
{
    char *file = jstringTostring(env, filename);
    bool b = quickLoad;
    jniEnv = env;
    app3d.loadObjModel(file, b);
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setRenderBufferSize(JNIEnv * env, jclass c, jint w, jint h)
{
    app3d.setRenderBufferSize(w,h);
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_frame(JNIEnv * env, jclass c)
{
    app3d.frame();
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_reset(JNIEnv * env, jclass c)
{
    app3d.reset();
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setDocDirectory(JNIEnv * env, jclass c, jstring docDir)
{
    char *doc = jstringTostring(env, docDir);
    app3d.setDocDirectory(doc);
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_rotate(JNIEnv * env, jclass c, jfloat deltaX, jfloat deltaY)
{
    app3d.rotate(deltaX,deltaY);
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_translate(JNIEnv * env, jclass c, jfloat deltaX, jfloat deltaY)
{
    app3d.translate(deltaX,deltaY);
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_scale(JNIEnv * env, jclass c, jfloat s)
{
    app3d.scale(s);
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setBackgroundColor(JNIEnv * env, jclass c, jfloat r, jfloat g, jfloat b, jfloat a)
{
    app3d.setBackgroundColor(r,g,b,a);
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setModelColor(JNIEnv * env, jclass c, jfloat r, jfloat g, jfloat b, jfloat a)
{
    app3d.setModelColor(r,g,b,a);
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setCameraPosition(JNIEnv * env, jclass c, jfloat x, jfloat y, jfloat z)
{
    app3d.setCameraPosition(x,y,z);
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setLightPosition(JNIEnv * env, jclass c, jfloat x, jfloat y, jfloat z)
{
    app3d.setLightPosition(x,y,z);
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setLigthDirection(JNIEnv * env, jclass c, jfloat x, jfloat y, jfloat z)
{
    app3d.setLigthDirection(x,y,z);
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setAmbientColor(JNIEnv * env, jclass c, jboolean r, jboolean g, jboolean b)
{
    app3d.setAmbientColor(r,g,b);
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_setDiffuseColor(JNIEnv * env, jclass c, jboolean r, jboolean g, jboolean b)
{
    app3d.setDiffuseColor(r,g,b);
}

JNIEXPORT void JNICALL Java_com_charlyzhang_gl2jni_GL2JNILib_enableTexture(JNIEnv * env, jclass c, jboolean flag)
{
}

