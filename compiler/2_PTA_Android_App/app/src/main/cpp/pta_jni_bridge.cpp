#include <jni.h>
#include <android/bitmap.h>
#include <cstdint>
#include <string>
#include <android/log.h>
#include <chrono>

#define LOG_TAG "PTANative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// External function declaration for the generated assembly
// This function is implemented in generated_logic.s
extern "C" void pta_process_image(
    int32_t width,
    int32_t height,
    uint8_t* pixel_in,
    uint8_t* pixel_out
);

extern "C" JNIEXPORT jlong JNICALL
Java_com_pta_engine_PtaNativeBridge_processImageWithPta(
    JNIEnv* env,
    jobject /* this */,
    jobject inputBitmap,
    jobject outputBitmap
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
        // Call the generated ARM64 assembly function
        pta_process_image(
            static_cast<int32_t>(inputInfo.width),
            static_cast<int32_t>(inputInfo.height),
            static_cast<uint8_t*>(inputPixels),
            static_cast<uint8_t*>(outputPixels)
        );

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