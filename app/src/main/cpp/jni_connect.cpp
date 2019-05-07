//
// Created by rostislav on 27.02.19.
//

#include <jni.h>
#include <thread>
#include "cameraEngine.h"
#include "CameraWrapper.h"

cameraEngine* cameraEngine1;
CameraWrapper* wrapper;


void imageProccesing::displayResultStream(float noise_level) {
    wrapper->displayNoiseLevel(noise_level);
}

void imageReader::imagePreview(jint *image, jint width, jint height){
    wrapper->drawImage(image, width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_dev_k504_cameraenginendk_MainActivity_OpenCamera(JNIEnv* env, jobject thiz, jboolean isCameraBack, jobject pInstance)
{
    wrapper = new CameraWrapper(env, pInstance);
    uint32_t w = 480;
    uint32_t h = 640;
    DisplayDimension dimension = DisplayDimension(w, h);
    cameraEngine1 = new cameraEngine(&dimension, isCameraBack);
}

extern "C" JNIEXPORT void JNICALL
        Java_dev_k504_cameraenginendk_MainActivity_cameraPreview(JNIEnv* env, jobject thiz)
{
    cameraEngine1->startPreview(true);

    std::thread prewiewHandler(&CameraWrapper::imageGeting, wrapper, cameraEngine1);
    prewiewHandler.detach();
}

extern "C" JNIEXPORT void JNICALL
Java_dev_k504_cameraenginendk_MainActivity_TakePhoto(JNIEnv* env, jobject thiz){
    std::thread photoHandler(&cameraEngine::onTakeImage, cameraEngine1);
    photoHandler.detach();
}

extern "C" JNIEXPORT void JNICALL
Java_dev_k504_cameraenginendk_MainActivity_deleteCamera(JNIEnv* env, jobject thiz)
{
    if(cameraEngine1)
    {
        cameraEngine1->deleteCamera();
        cameraEngine1 = nullptr;
        wrapper = nullptr;
    }
}