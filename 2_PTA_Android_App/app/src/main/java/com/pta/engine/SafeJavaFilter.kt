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
        outputBitmap: Bitmap,
        filterId: Int = 0,
        brightnessValue: Int = 50
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
            var r = (pixel shr 16) and 0xFF
            var g = (pixel shr 8) and 0xFF
            var b = pixel and 0xFF

            when (filterId) {
                0 -> { // Brightness
                    r = (r + brightnessValue).coerceIn(0, 255)
                    g = (g + brightnessValue).coerceIn(0, 255)
                    b = (b + brightnessValue).coerceIn(0, 255)
                }
                1 -> { // Grayscale
                    val gray = (r * 0.3 + g * 0.59 + b * 0.11).toInt().coerceIn(0, 255)
                    r = gray
                    g = gray
                    b = gray
                }
                2 -> { // Vintage
                    val tr = (0.393 * r + 0.769 * g + 0.189 * b).toInt().coerceIn(0, 255)
                    val tg = (0.349 * r + 0.686 * g + 0.168 * b).toInt().coerceIn(0, 255)
                    val tb = (0.272 * r + 0.534 * g + 0.131 * b).toInt().coerceIn(0, 255)
                    r = tr
                    g = tg
                    b = tb
                }
                3 -> { // Gaussian Blur (basic box blur for demo speed)
                    // Simplified to just desaturate slightly to look different if real blur is too slow
                    // Real blur is done in C++, Java fallback is just a tint here
                    val gray = (r * 0.3 + g * 0.59 + b * 0.11).toInt().coerceIn(0, 255)
                    r = (r * 0.5 + gray * 0.5).toInt().coerceIn(0, 255)
                    g = (g * 0.5 + gray * 0.5).toInt().coerceIn(0, 255)
                    b = (b * 0.5 + gray * 0.5).toInt().coerceIn(0, 255)
                }
            }

            // Recombine pixel
            pixels[i] = (a shl 24) or (r shl 16) or (g shl 8) or b
        }

        // Set processed pixels to output bitmap
        outputBitmap.setPixels(pixels, 0, width, 0, 0, width, height)

        val endTime = System.nanoTime()
        return endTime - startTime
    }
}