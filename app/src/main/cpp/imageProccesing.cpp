//
// Created by rostislav on 05.04.19.
//

#include <iostream>
#include <vector>
#include <string>

#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/string_util.h"
#include "tensorflow/lite/core/api/op_resolver.h"

#include <string>

#include "utils.h"
#include "imageProccesing.h"

// Constructor for class imageProccesing
// @param: size_batch = size of window batch (size_batch * size_batch);
//         count_batch = count of batch
imageProccesing::imageProccesing(int size_batch, int count_batch)
{
    batchSize = size_batch;
    countBatch = count_batch;
}

// function for calculate batch array and return this to SDK
// @param: Image for process
void imageProccesing::calculateBatchArray(uint8_t *image, int width) {
    double mean = 0;
    double summ = 0;

    float noise_sum = 0.0f;

    tflite::StderrReporter reporter;
    const char* model_char = "/sdcard/develop/NoiseNet/NoiseNet.tflite";
    std::unique_ptr<tflite::FlatBufferModel> model(tflite::FlatBufferModel::BuildFromFile(model_char));

    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder builder(*model, resolver);

    int im_y = 0;
    int im_x = 0;
    if(builder(&interpreter) == kTfLiteOk) {
        interpreter->AllocateTensors();

        input_1 = interpreter->typed_input_tensor<float>(0);
        input_2 = interpreter->typed_input_tensor<float>(1);


        validModel = true;

        interpreter->UseNNAPI(false);
        input_2[0] = 100.0f;

        /**
         * Taken block 32 * 32 with delay 1 px to remove spatial correlation
         * 64 is 32*delay(2)
         */

        for (int i = 0; i < countBatch; i++) {
            for (int j = im_y; j < (batchSize + im_y); j+=2) {
                for(int k = im_x; k < (batchSize + im_x); k+=2) {
                    summ += image[(j * width) + k];
                }
            }
            im_x += 64;

            if(im_x + 64 > width) im_y = im_y + 64;

            mean = summ / (batchSize * batchSize);
            summ = 0;
            for (int j = 0; j < batchSize * batchSize; j+=2) {
                input_1[j] = (float) (image[j + (i * 32)] / mean);
            }
            k_normalize = (float) mean;
            interpreter->Invoke();

            output = interpreter->typed_output_tensor<float>(0);

            input_2[0] = output[0]*k_normalize;
            noise_sum += input_2[0];
            LOGI("Info");
        }

        displayResultStream(noise_sum/countBatch);
    }
}