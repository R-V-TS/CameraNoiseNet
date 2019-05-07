//
// Created by rostislav on 05.04.19.
//

#ifndef CAMERAENGINE_IMAGEPROCCESING_H
#define CAMERAENGINE_IMAGEPROCCESING_H


#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/string_util.h"
#include "tensorflow/lite/core/api/op_resolver.h"
#include <media/NdkImage.h>
#include <jni.h>

class imageProccesing {

public:
    imageProccesing(int size_batch, int count_batch);
    ~imageProccesing();

    void calculateBatchArray(uint8_t *image, int width);
    void displayResultStream(float noise_level); // Function for display stream result. Realization in jni_connect

private:
    std::unique_ptr<tflite::Interpreter> interpreter;

    float *input_1;
    float *input_2;
    float *output;

    bool validModel; //flag for model loader

    int batchSize;
    int countBatch;
    int size;
    uint8_t *arrayBatch;
    float k_normalize;
};


#endif //CAMERAENGINE_IMAGEPROCCESING_H
