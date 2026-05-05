# Portable Typed Assembly (PTA) Compiler

PTA is a custom "Low-Level DSL" (Domain Specific Language) compiler designed for high-performance image processing. The goal is to provide a syntax that resembles Assembly for direct hardware control, but ensures "Safety" and "Portability" by translating it into architecture-specific optimizations (like ARM64).

This repository contains:
1. **Desktop Compiler** (`compiler/1_PTA_Desktop_Compiler/`): A C++17 compiler that lexes, optimizes, and emits our custom TASM assembly code.
2. **Android Application** (`compiler/2_PTA_Android_App/`): An Android Studio project bridging the compiled outputs with a responsive image processing UI via JNI and NDK.

## Key Features
* **Safety-Stripping Optimization:** The compiler identifies redundant software "saturation clamps" (CMP + MOV_IF_GT) and strips them.
* **Hardware-Level Saturation (NEON SIMD):** Bypasses software-level overflow checks by directly targeting the ARM64 `uqadd` (Unsigned Saturating Add) instruction. This provides flawless, zero-overhead brightness clamping and prevents grainy pixels.
* **Dynamic Register Allocation:** Maps "infinite" virtual registers (`%r1`, `%r2`) to physical ARM64 registers (e.g., `x0`, `v5.4s`), safely distinguishing between 64-bit address pointers and 32-bit SIMD math data.
* **High-Speed Execution:** Designed to be optimal for mobile processors, reducing branch mispredictions and boosting image processing speed (typically ~0.4ms to 6ms depending on resolution).

## Project Structure
```text
compiler/
├── 1_PTA_Desktop_Compiler/   # C++ Compiler source code (Lexer, Optimizer, Mapper, Emitter)
├── 2_PTA_Android_App/        # Android JNI Bridge & UI
├── PTA_LANGUAGE_DESIGN.txt   # Detailed specification of the PTA Language
├── OPTIMIZER_STRATEGY.txt    # Breakdown of the "Safety-Stripping" technique
└── TEAM_DIVISION.txt         # PBL Team Workload breakdown
```

## Installation and Build Instructions

### 1. Building the Desktop Compiler (C++)
**Prerequisites:** 
- CMake (3.16+)
- A C++17 compatible compiler (GCC, Clang, or MSVC)

**Steps:**
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
4. Run a demo filter script (if available):
   ```bash
   ./bin/pta_compiler ../scripts/demo_filter.tasm
   ```

### 2. Building the Android App
**Prerequisites:**
- Android Studio
- Android SDK & NDK (Native Development Kit)
- JDK 11+

**Steps:**
1. Open **Android Studio**.
2. Select **File > Open...** and navigate to `compiler/2_PTA_Android_App`.
3. Wait for Gradle to sync and download required dependencies. Ensure your NDK path is correctly configured in Android Studio (via SDK Manager).
4. Connect an Android device (ARM64 recommended) or start an Emulator.
5. Click **Run > Run 'app'** to install and launch the application.

## Team and Architecture
This project was developed under a PBL (Project-Based Learning) framework, with a 4-member team managing different stages of the compiler pipeline:
1. **Front-End Architect**: Lexical Analysis and UI Design.
2. **Middle-End Analyst**: "Safety-Stripping" Optimization and Benchmarking.
3. **Back-End Specialist**: Register Mapping and JNI Bridge.
4. **Generator Lead**: Assembly Emission and Filter Generation.
