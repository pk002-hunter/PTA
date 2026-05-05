package com.pta.engine

/**
 * Native bridge for PTA (Portable Typed Assembly) engine
 * Provides JNI interface to execute safety-stripped ARM64 assembly
 */
class PtaNativeBridge {

    companion object {
        // Load the native library
        init {
            System.loadLibrary("ptanative")
        }
    }

    /**
     * Process image using PTA-generated ARM64 assembly
     * @param inputBitmap Input image bitmap
     * @param outputBitmap Output image bitmap (will be modified)
     * @return Processing time in nanoseconds for performance measurement
     */
    external fun processImageWithPta(
        inputBitmap: android.graphics.Bitmap,
        outputBitmap: android.graphics.Bitmap
    ): Long

    /**
     * Get version information about the PTA native engine
     */
    external fun getNativeVersion(): String
}