#include <jni.h>
#include <android/bitmap.h>
#include <cstdint>
#include <string>
#include <android/log.h>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

#define LOG_TAG "PTANative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" void pta_process_image(
    int32_t width,
    int32_t height,
    uint8_t* pixel_in,
    uint8_t* pixel_out,
    int32_t brightness
);

extern "C" JNIEXPORT jlong JNICALL
Java_com_pta_engine_PtaNativeBridge_processImageWithPta(
    JNIEnv* env,
    jobject /* this */,
    jobject inputBitmap,
    jobject outputBitmap,
    jint filterId,
    jint brightness,
    jboolean isParallel
) {
    AndroidBitmapInfo inputInfo, outputInfo;
    void* inputPixels = nullptr;
    void* outputPixels = nullptr;

    // Get bitmap information
    if (AndroidBitmap_getInfo(env, inputBitmap, &inputInfo) != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("Failed to get input bitmap info");
        return -1;
    }

    if (AndroidBitmap_getInfo(env, outputBitmap, &outputInfo) != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("Failed to get output bitmap info");
        return -1;
    }

    // Validate bitmap format and dimensions
    if (inputInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Input bitmap must be RGBA_8888 format");
        return -1;
    }

    if (outputInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Output bitmap must be RGBA_8888 format");
        return -1;
    }

    if (inputInfo.width != outputInfo.width || inputInfo.height != outputInfo.height) {
        LOGE("Input and output bitmaps must have same dimensions");
        return -1;
    }

    // Lock bitmaps to get direct pixel access
    if (AndroidBitmap_lockPixels(env, inputBitmap, &inputPixels) != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("Failed to lock input bitmap pixels");
        return -1;
    }

    if (AndroidBitmap_lockPixels(env, outputBitmap, &outputPixels) != ANDROID_BITMAP_RESULT_SUCCESS) {
        AndroidBitmap_unlockPixels(env, inputBitmap);
        LOGE("Failed to lock output bitmap pixels");
        return -1;
    }

    // Measure processing time
    auto startTime = std::chrono::high_resolution_clock::now();

    try {
        int32_t width = inputInfo.width;
        int32_t height = inputInfo.height;
        uint8_t* pIn = static_cast<uint8_t*>(inputPixels);
        uint8_t* pOut = static_cast<uint8_t*>(outputPixels);
        
        int num_threads = 1;
        if (isParallel) {
            num_threads = std::thread::hardware_concurrency();
            if (num_threads <= 0) num_threads = 4;
        }
        
        auto process_chunk = [&](int start_y, int end_y) {
            if (filterId == 0) {
                // Assembly Brightness (PTA Core)
                int chunk_height = end_y - start_y;
                uint8_t* chunk_in = pIn + (start_y * width * 4);
                uint8_t* chunk_out = pOut + (start_y * width * 4);
                pta_process_image(width, chunk_height, chunk_in, chunk_out, brightness);
            } else {
                // C++ Filters
                for (int y = start_y; y < end_y; ++y) {
                    for (int x = 0; x < width; ++x) {
                        int idx = (y * width + x) * 4;
                        uint8_t r = pIn[idx];
                        uint8_t g = pIn[idx+1];
                        uint8_t b = pIn[idx+2];
                        uint8_t a = pIn[idx+3];
                        
                        if (filterId == 1) { // Grayscale
                            uint8_t gray = (r * 77 + g * 150 + b * 29) >> 8;
                            pOut[idx] = gray;
                            pOut[idx+1] = gray;
                            pOut[idx+2] = gray;
                            pOut[idx+3] = a;
                        } else if (filterId == 2) { // Vintage
                            int tr = (r * 100 + g * 196 + b * 48) >> 8;
                            int tg = (r * 89 + g * 175 + b * 43) >> 8;
                            int tb = (r * 69 + g * 136 + b * 33) >> 8;
                            pOut[idx] = tr > 255 ? 255 : tr;
                            pOut[idx+1] = tg > 255 ? 255 : tg;
                            pOut[idx+2] = tb > 255 ? 255 : tb;
                            pOut[idx+3] = a;
                        } else if (filterId == 3) { // Gaussian Blur (3x3 Box Blur)
                            int sumR=0, sumG=0, sumB=0, count=0;
                            for(int dy=-1; dy<=1; dy++){
                                for(int dx=-1; dx<=1; dx++){
                                    int ny = y+dy;
                                    int nx = x+dx;
                                    if(ny>=0 && ny<height && nx>=0 && nx<width){
                                        int nidx = (ny*width + nx)*4;
                                        sumR += pIn[nidx];
                                        sumG += pIn[nidx+1];
                                        sumB += pIn[nidx+2];
                                        count++;
                                    }
                                }
                            }
                            pOut[idx] = sumR/count;
                            pOut[idx+1] = sumG/count;
                            pOut[idx+2] = sumB/count;
                            pOut[idx+3] = a;
                        }
                    }
                }
            }
        };

        std::vector<std::thread> threads;
        int chunk_size = height / num_threads;
        for (int i = 0; i < num_threads; ++i) {
            int start_y = i * chunk_size;
            int end_y = (i == num_threads - 1) ? height : start_y + chunk_size;
            threads.emplace_back(process_chunk, start_y, end_y);
        }
        for (auto& t : threads) {
            t.join();
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

        LOGI("PTA processing completed in %lld ns", duration.count());

        // Unlock bitmaps
        AndroidBitmap_unlockPixels(env, inputBitmap);
        AndroidBitmap_unlockPixels(env, outputBitmap);

        return duration.count();

    } catch (const std::exception& e) {
        LOGE("Exception during PTA processing: %s", e.what());
        AndroidBitmap_unlockPixels(env, inputBitmap);
        AndroidBitmap_unlockPixels(env, outputBitmap);
        return -1;
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_pta_engine_PtaNativeBridge_getNativeVersion(
    JNIEnv* env,
    jobject /* this */
) {
    return env->NewStringUTF("PTA Native Engine v1.0 - Safety-Stripped ARM64");
}