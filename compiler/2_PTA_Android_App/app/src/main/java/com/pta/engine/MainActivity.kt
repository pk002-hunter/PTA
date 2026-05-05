package com.pta.engine

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Bundle
import android.widget.ImageView
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import java.io.InputStream

class MainActivity : AppCompatActivity() {

    private lateinit var textureViewJava: ImageView
    private lateinit var textureViewPta: ImageView
    private lateinit var btnUploadImage: Button
    private lateinit var btnProcess: Button
    private lateinit var tvJavaTime: TextView
    private lateinit var tvPtaTime: TextView

    private var javaBitmap: Bitmap? = null
    private var ptaBitmap: Bitmap? = null

    private val ptaNativeBridge = PtaNativeBridge()

    // Image picker launcher
    private val pickImageLauncher = registerForActivityResult(ActivityResultContracts.GetContent()) { uri: Uri? ->
        uri?.let { loadSelectedImage(it) }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        initializeViews()
        setupClickListeners()
    }

    private fun initializeViews() {
        textureViewJava = findViewById(R.id.textureViewJava)
        textureViewPta = findViewById(R.id.textureViewPta)
        btnUploadImage = findViewById(R.id.btnUploadImage)
        btnProcess = findViewById(R.id.btnProcess)
        tvJavaTime = findViewById(R.id.tvJavaTime)
        tvPtaTime = findViewById(R.id.tvPtaTime)
        
        btnProcess.isEnabled = false
    }

    private fun setupClickListeners() {
        btnUploadImage.setOnClickListener {
            pickImageLauncher.launch("image/*")
        }

        btnProcess.setOnClickListener {
            processImage()
        }
    }

    private fun loadSelectedImage(uri: Uri) {
        try {
            val inputStream: InputStream? = contentResolver.openInputStream(uri)
            val selectedBitmap = BitmapFactory.decodeStream(inputStream)
            inputStream?.close()

            if (selectedBitmap == null) {
                Toast.makeText(this, "Failed to decode image", Toast.LENGTH_SHORT).show()
                return
            }

            // Ensure bitmap is in ARGB_8888 format and mutable for JNI
            // PTA JNI specifically checks for ANDROID_BITMAP_FORMAT_RGBA_8888
            val argb8888Bitmap = selectedBitmap.copy(Bitmap.Config.ARGB_8888, true)
            
            javaBitmap = argb8888Bitmap.copy(Bitmap.Config.ARGB_8888, true)
            ptaBitmap = argb8888Bitmap.copy(Bitmap.Config.ARGB_8888, true)

            textureViewJava.setImageBitmap(javaBitmap)
            textureViewPta.setImageBitmap(null) // Clear previous result
            
            tvJavaTime.text = "Java: -- ms"
            tvPtaTime.text = "PTA: -- ms"
            
            btnProcess.isEnabled = true
            Toast.makeText(this, "Image loaded: ${argb8888Bitmap.width}x${argb8888Bitmap.height}", Toast.LENGTH_SHORT).show()

        } catch (e: Exception) {
            Toast.makeText(this, "Error loading image: ${e.message}", Toast.LENGTH_LONG).show()
        }
    }

    private fun processImage() {
        val jBitmap = javaBitmap ?: return
        val pBitmap = ptaBitmap ?: return

        // Process with Java implementation (using the optimized version for fair comparison)
        val javaTime = SafeJavaFilter.processImageWithJavaOptimized(jBitmap, jBitmap)
        tvJavaTime.text = "Java: ${javaTime / 1_000_000} ms"
        textureViewJava.setImageBitmap(jBitmap)

        // Process with PTA implementation
        // Note: we pass the original jBitmap as input and pBitmap as output
        val ptaTime = ptaNativeBridge.processImageWithPta(jBitmap, pBitmap)
        
        if (ptaTime > 0) {
            tvPtaTime.text = "PTA: ${ptaTime / 1_000_000} ms"
            textureViewPta.setImageBitmap(pBitmap)
        } else {
            tvPtaTime.text = "PTA: Error"
        }

        // Show performance comparison
        if (ptaTime > 0 && javaTime > 0) {
            val speedup = javaTime.toDouble() / ptaTime.toDouble()
            val speedupText = String.format("%.2f", speedup)
            Toast.makeText(this, "PTA is ${speedupText}x faster than Java", Toast.LENGTH_LONG).show()
        }
    }
}