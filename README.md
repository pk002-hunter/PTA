# Portable Typed Assembly (PTA) Compiler

PTA is a custom "Low-Level DSL" (Domain Specific Language) compiler engineered specifically for ultra high-performance image processing on mobile devices. The architecture allows developers to write code that resembles Assembly for direct, unhindered hardware control, while ensuring "Safety" by compiling it into deeply optimized, architecture-specific instructions (ARM64).

This repository contains two primary components:
1. **Desktop Compiler** (`compiler/1_PTA_Desktop_Compiler/`): A C++17 compiler that lexes, optimizes, and emits our custom TASM assembly code.
2. **Android Application** (`compiler/2_PTA_Android_App/`): The front-end Android Studio project connecting the compiled PTA outputs with a responsive, professional-grade benchmarking UI via a native JNI/NDK bridge.

---

## 🚀 Key Technological Features

### 1. Multi-Core Hardware Parallelism
The Android integration bypasses standard single-threaded Java bottlenecks. By querying `std::thread::hardware_concurrency()` at the C++ level, the engine horizontally chunks incoming image buffers and spawns simultaneous native OS threads to process the image. This typically utilizes **100% of available Snapdragon Oryon cores** concurrently, delivering up to an 800% speed increase.

### 2. Live Linux Sysfs CPU Dashboard
The application features a real-time hardware monitor embedded into the UI. It runs a background timer that directly polls Android's deep kernel files (`/sys/devices/system/cpu/cpuX/cpufreq/scaling_cur_freq`) to calculate and display live clock-speed percentages across all 8 cores.

### 3. Native PDF Architecture Reporting
Includes a "Show Performance Report" capability that uses `android.graphics.pdf.PdfDocument` to generate and draw a formal, grouped bar chart inside a `.pdf` file. This visual report contrasts the 8-core PTA execution against the single-threaded Java execution, outputting it directly to the user's `Downloads` directory.

### 4. Zero-Overhead Hardware Saturation (NEON SIMD)
Instead of relying on software-level bounds checking, the PTA engine maps directly to the ARM64 `uqadd` (Unsigned Saturating Add) instruction. By operating at the byte level (`.8b`), each core securely processes 8 color channels per clock cycle without risking pixel integer overflow (grainy noise).

### 5. Intelligent Memory Management
Massive files (like 50-Megapixel Panoramas) are protected against Android's strict `OutOfMemoryError` heap limits via a dynamic `BitmapFactory.Options` pre-load downscaling algorithm.

---

## 📂 Project Structure
```text
/
├── compiler/
│   ├── 1_PTA_Desktop_Compiler/   # C++ Compiler source (Lexer, Optimizer, Mapper)
│   ├── 2_PTA_Android_App/        # Android Benchmark UI & JNI NDK Bridge
│   ├── pta_vs_java_core_report.md# Markdown documentation on architecture theory
│   ├── PTA_LANGUAGE_DESIGN.txt   # Core DSL language specifications
│   ├── OPTIMIZER_STRATEGY.txt    # Breakdown of "Safety-Stripping" techniques
│   └── TEAM_DIVISION.txt         # Academic Team Workload breakdown
└── requirements.txt              # System dependency manifest
```

---

## 🛠 Installation and Build Instructions

### 1. System Requirements (`requirements.txt`)
Before building, ensure your environment meets the criteria detailed in the `requirements.txt` file located in the root directory. Primarily:
- CMake (3.16+)
- A C++17 compatible compiler (GCC, Clang, or MSVC)
- Android Studio with Android NDK & SDK installed.

### 2. Building the Desktop Compiler (C++)
1. Open a terminal and navigate to the compiler directory:
   ```bash
   cd compiler/1_PTA_Desktop_Compiler
   ```
2. Create a build directory and configure the project:
   ```bash
   mkdir build && cd build
   cmake ..
   ```
3. Build the compiler executable:
   ```bash
   cmake --build . --target pta_compiler
   ```

### 3. Building and Running the Android App
1. Open **Android Studio**.
2. Select **File > Open...** and navigate into `compiler/2_PTA_Android_App`.
3. Wait for Gradle to fully sync. Ensure that your **NDK path** is correctly configured via the SDK Manager, as this project heavily relies on C++ compilation.
4. Connect a physical Android device (ARM64 recommended) for the most accurate multi-core performance numbers.
5. Click **Run > Run 'app'** to install and launch the application.

---

## 👥 Academic Team Division
Developed under a PBL (Project-Based Learning) framework, four distinct architectural roles were established:
1. **Front-End Architect**: Lexical Analysis, Tokenization, and Android UI Design.
2. **Middle-End Analyst**: "Safety-Stripping", JNI Filter Expansions (Blur, Sepia), and Benchmarking logic.
3. **Back-End Specialist**: Register Mapping, Multi-Threading Architecture, and Real-time `sysfs` Polling.
4. **Generator Lead**: Assembly Emission, SIMD instruction design, and PDF Graph Reporting.
