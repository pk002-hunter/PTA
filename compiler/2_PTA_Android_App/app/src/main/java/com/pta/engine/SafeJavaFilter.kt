package com.pta.engine

import android.graphics.Bitmap

/**
 * Standard Java implementation of the image filter for comparison
 * This is the "safe" version with bounds checking and standard Java math
 */
object SafeJavaFilter {

    /**
     * Process image using standard Java implementation
     * @param inputBitmap Input image bitmap
     * @param outputBitmap Output image bitmap (will be modified)
     * @return Processing time in nanoseconds for performance comparison
     */
    fun processImageWithJava(
        inputBitmap: Bitmap,
        outputBitmap: Bitmap
    ): Long {
        val startTime = System.nanoTime()

        val width = inputBitmap.width
        val height = inputBitmap.height

        // Ensure bitmaps have same dimensions
        require(width == outputBitmap.width && height == outputBitmap.height) {
            "Input and output bitmaps must have same dimensions"
        }

        // Process each pixel using safe Java implementation
        for (y in 0 until height) {
            for (x in 0 until width) {
                val pixel = inputBitmap.getPixel(x, y)

                // Extract RGB components
                val a = android.graphics.Color.alpha(pixel)
                val r = android.graphics.Color.red(pixel)
                val g = android.graphics.Color.green(pixel)
                val b = android.graphics.Color.blue(pixel)

                // Apply brightness adjustment (add 50 to each component)
                val newR = (r + 50).coerceIn(0, 255)
                val newG = (g + 50).coerceIn(0, 255)
                val newB = (b + 50).coerceIn(0, 255)

                // Set the new pixel with safety checks
                val newPixel = android.graphics.Color.argb(a, newR, newG, newB)
                outputBitmap.setPixel(x, y, newPixel)
            }
        }

        val endTime = System.nanoTime()
        return endTime - startTime
    }

    /**
     * Alternative implementation using direct pixel access for better performance
     * but still with Java safety checks
     */
    fun processImageWithJavaOptimized(
        inputBitmap: Bitmap,
        outputBitmap: Bitmap
    ): Long {
        val startTime = System.nanoTime()

        val width = inputBitmap.width
        val height = inputBitmap.height

        // Use direct pixel arrays for better performance
        val pixels = IntArray(width * height)
        inputBitmap.getPixels(pixels, 0, width, 0, 0, width, height)

        // Process each pixel
        for (i in pixels.indices) {
            val pixel = pixels[i]

            // Extract RGB components
            val a = (pixel shr 24) and 0xFF
            val r = (pixel shr 16) and 0xFF
            val g = (pixel shr 8) and 0xFF
            val b = pixel and 0xFF

            // Apply brightness adjustment with safety checks
            val newR = (r + 50).coerceIn(0, 255)
            val newG = (g + 50).coerceIn(0, 255)
            val newB = (b + 50).coerceIn(0, 255)

            // Recombine pixel
            pixels[i] = (a shl 24) or (newR shl 16) or (newG shl 8) or newB
        }

        // Set processed pixels to output bitmap
        outputBitmap.setPixels(pixels, 0, width, 0, 0, width, height)

        val endTime = System.nanoTime()
        return endTime - startTime
    }
}