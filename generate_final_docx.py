from docx import Document
from docx.shared import Pt, Inches, RGBColor
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml.ns import qn
import os

def create_pta_doc_final():
    doc = Document()

    # Define style helper
    def set_font(run, size, bold=False):
        run.font.name = 'Times New Roman'
        run._element.rPr.rFonts.set(qn('w:ascii'), 'Times New Roman')
        run.font.size = Pt(size)
        run.font.bold = bold
        run.font.color.rgb = RGBColor(0, 0, 0) # Strictly Black

    def add_heading(text, level):
        h = doc.add_heading(text, level=level)
        for run in h.runs:
            size = 16 if level == 1 else 14
            set_font(run, size, True)
        h.alignment = WD_ALIGN_PARAGRAPH.LEFT
        return h

    def add_paragraph(text, size=12, indent=True):
        p = doc.add_paragraph()
        if indent:
            p.paragraph_format.first_line_indent = Inches(0.5)
        p.alignment = WD_ALIGN_PARAGRAPH.JUSTIFY
        run = p.add_run(text)
        set_font(run, size)
        return p

    def add_image(image_path, caption):
        if os.path.exists(image_path):
            doc.add_picture(image_path, width=Inches(5))
            p = doc.add_paragraph()
            p.alignment = WD_ALIGN_PARAGRAPH.CENTER
            run = p.add_run(f"Figure: {caption}")
            set_font(run, 10, True)
        else:
            p = doc.add_paragraph()
            run = p.add_run(f"[Image Missing: {image_path}]")
            set_font(run, 10, True)

    # --- 5. Abstract ---
    add_heading("Abstract", 1)
    abstract_text = (
        "This project presents the comprehensive design, development, and rigorous evaluation of the Portable Typed Assembly (PTA) Compiler, "
        "a cutting-edge Domain Specific Language (DSL) specifically engineered for the ultra-high-performance demands of mobile image processing. "
        "The fundamental challenge addressed in this work is the inherent performance bottleneck found in standard managed languages like Java and Kotlin, "
        "which often rely on the Android Runtime (ART) and heavy software-level safety checks, leaving modern multi-core mobile hardware like the "
        "Snapdragon 8 Gen 4 significantly underutilized. Our innovative solution introduces a custom assembly-inspired syntax that allows developers "
        "to maintain fine-grained control over hardware registers while abstracting the complexities of cross-platform porting. A key architectural "
        "breakthrough is our 'Safety-Stripping' optimizer, which programmatically identifies redundant overflow and bounds checks and replaces them "
        "with hardware-native SIMD saturation instructions (ARM64 NEON). Developed using a robust C++17 desktop compiler pipeline and integrated "
        "into a responsive Android environment via an optimized JNI/NDK bridge, the system achieves performance gains of up to 800% on Oryon cores. "
        "This project serves as a scalable prototype for future low-level mobile optimizations, bridging the gap between high-level application "
        "logic and the raw computational power of modern mobile silicon."
    )
    add_paragraph(abstract_text)

    # --- 6. Table of Contents ---
    doc.add_page_break()
    add_heading("Table of Contents", 1)
    toc_lines = [
        "Abstract....................................................................i",
        "Chapter 1: Introduction.....................................................1",
        "  1.1 Introduction..........................................................1",
        "  1.2 Background / Motivation...............................................2",
        "  1.3 Problem Statement.....................................................3",
        "  1.4 Objectives & Methodology..............................................4",
        "  1.5 Project Organization..................................................5",
        "Chapter 2: Proposed System..................................................6",
        "  2.1 System Overview.......................................................6",
        "  2.2 System Workflow / Interaction Flow....................................7",
        "  2.3 Key Features / Innovations............................................8",
        "Chapter 3: Hardware & Software Requirements.................................9",
        "  3.1 Software Requirements.................................................9",
        "  3.2 Hardware Requirements................................................10",
        "  3.3 Security / Reliability...............................................11",
        "Chapter 4: System Architecture..............................................12",
        "  4.1 High-Level Architecture..............................................12",
        "  4.2 Data Flow / Diagrams.................................................13",
        "Chapter 5: System Design / Modules..........................................14",
        "  5.1 Module 1: Desktop Compiler Pipeline..................................14",
        "  5.2 Module 2: Android NDK & JNI Bridge...................................16",
        "  5.3 Module 3: Performance Reporting Engine................................18",
        "Chapter 6: Team Contribution................................................20",
        "Chapter 7: Limitations......................................................22",
        "Chapter 8: Enhancements / Future Work.......................................23",
        "Chapter 9: Conclusion.......................................................24",
        "References..................................................................25"
    ]
    for line in toc_lines:
        p = doc.add_paragraph()
        run = p.add_run(line)
        set_font(run, 12)

    # --- MAIN BODY ---
    doc.add_page_break()

    # Chapter 1
    add_heading("Chapter 1: Introduction", 1)
    add_heading("1.1 Introduction", 2)
    add_paragraph(
        "The Portable Typed Assembly (PTA) Compiler is a specialized low-level development environment created to overcome the limitations "
        "of high-level mobile programming. By providing a syntax that mirrors the direct control of Assembly language while maintaining "
        "type safety and portability, PTA allows developers to write optimized image processing kernels that can be seamlessly deployed "
        "across ARM64 devices. This project explores the full lifecycle of a domain-specific language, from lexical analysis to silicon execution."
    )
    add_heading("1.2 Background / Motivation", 2)
    add_paragraph(
        "Modern mobile processors, such as the Snapdragon 8 Gen 4 with its custom Oryon cores, possess computational throughput comparable to "
        "desktop processors. However, most mobile applications only utilize a fraction of this power due to the constraints of the Java Virtual Machine "
        "and the Android Runtime. Our motivation was to bypass these high-level bottlenecks and allow for direct hardware saturation, especially "
        "in compute-intensive tasks like real-time filter application and high-resolution image manipulation."
    )
    add_heading("1.3 Problem Statement", 2)
    add_paragraph(
        "Existing image processing frameworks on Android often suffer from 'The Overhead Crisis.' Standard code performs defensive checks "
        "for every pixel operation (clamping, bounds checking, etc.), which significantly slows down the CPU pipeline through branch mispredictions. "
        "Additionally, many applications fail to utilize the SIMD (Single Instruction, Multiple Data) capabilities of ARM processors effectively, "
        "leading to performance that is several orders of magnitude slower than what the hardware is capable of achieving."
    )
    add_heading("1.4 Objectives & Methodology", 2)
    add_paragraph(
        "The primary objective of this project is to implement a cross-compiler that translates a custom PTA DSL into vectorized ARM64 code. "
        "Our methodology relies on the concept of 'Safety-Stripping,' where software logic is replaced by hardware features. We utilize a multi-threaded "
        "architecture at the JNI layer to ensure that processing tasks are distributed evenly across all available CPU cores, maximizing throughput "
        "and minimizing latency for the end-user."
    )
    add_heading("1.5 Project Organization", 2)
    add_paragraph(
        "This document is structured to provide a comprehensive look at the PTA ecosystem. We begin with the theoretical introduction, "
        "move into the proposed architectural solutions, detail the hardware and software requirements, and provide an in-depth analysis "
        "of each compiler module. Finally, we discuss team contributions, current system limitations, and future enhancement opportunities "
        "to scale the project into a production-ready compiler suite."
    )

    # Chapter 2
    doc.add_page_break()
    add_heading("Chapter 2: Proposed System", 1)
    add_heading("2.1 System Overview", 2)
    add_paragraph(
        "The proposed PTA system is a dual-component architecture consisting of a high-performance C++ Desktop Compiler and a native-enabled "
        "Android Benchmark Application. The system allows for the creation of .tasm (PTA Assembly) scripts which are then compiled into bare-metal "
        "ARM64 machine code, bypassing the standard Android Dalvik/ART runtime overhead entirely to achieve maximum speed."
    )
    add_heading("2.2 System Workflow", 2)
    add_paragraph(
        "The interaction flow starts with the development of image kernels in the PTA syntax. These scripts are processed by the Desktop Compiler "
        "to produce highly optimized assembly (.s) files. These files are then integrated into the Android project using CMake and the NDK. "
        "When the user selects a filter in the Android app, the system dynamically locks the bitmap memory and passes it to the native PTA engine "
        "for multi-core parallel processing."
    )
    add_image("phases.png", "System Development Phases and Workflow")
    add_heading("2.3 Key Features / Innovations", 2)
    add_paragraph(
        "Our most significant innovation is the 'Safety-Stripping' optimization pass. Most compilers add defensive code to prevent pixel "
        "overflow, but the PTA compiler identifies these patterns and 'strips' them out, instead leveraging the ARM64 NEON 'uqadd' instruction. "
        "This instruction performs saturating arithmetic at the hardware level, ensuring that pixel values never wrap around, while removing the "
        "need for expensive conditional branch instructions in the code."
    )

    # Chapter 3
    doc.add_page_break()
    add_heading("Chapter 3: Hardware & Software Requirements", 1)
    add_heading("3.1 Software Requirements", 2)
    add_paragraph(
        "The development environment requires a C++17 compliant compiler (such as GCC 9+ or Clang 11+) for the desktop component. For mobile integration, "
        "Android Studio Jellyfish or later is required, along with the Android NDK (Native Development Kit) version r25c or higher. The build system is "
        "managed via CMake 3.16+, ensuring cross-platform compatibility and efficient native code linking during the compilation process."
    )
    add_heading("3.2 Hardware Requirements", 2)
    add_paragraph(
        "To achieve the performance metrics described in this report, an ARM64-v8a compatible Android device is required. The system is specifically "
        "optimized for high-end mobile silicon such as the Snapdragon 8 Gen 4, which features dedicated Oryon performance cores. A minimum of 8GB "
        "of RAM is recommended to handle large image buffers (up to 50 megapixels) without triggering the system's Low Memory Killer."
    )
    add_heading("3.3 Security / Reliability", 2)
    add_paragraph(
        "Security and reliability are maintained through strict memory boundary enforcement at the JNI (Java Native Interface) layer. By using "
        "AndroidBitmap_lockPixels, the system ensures that the native code has exclusive, safe access to the pixel buffer. Additionally, the compiler "
        "performs register tracking to prevent common assembly-level errors such as stack corruption or unauthorized memory access beyond the bitmap."
    )

    # Chapter 4
    doc.add_page_break()
    add_heading("Chapter 4: System Architecture", 1)
    add_heading("4.1 High-Level Architecture", 2)
    add_paragraph(
        "The architecture follows a decoupled modular pattern. The 'Control Layer' resides in Kotlin, managing the UI and image lifecycle. The 'Execution Layer' "
        "resides in C++ and ARM64 Assembly, handling the raw mathematical transformations. This separation ensures that the UI remains responsive "
        "while the heavy computational tasks are offloaded to specialized background threads running on the phone's performance cores."
    )
    add_image("architecture.png", "High-Level System Architecture and Component Interaction")
    add_heading("4.2 Data Flow / Diagrams", 2)
    add_paragraph(
        "Data flow is strictly unidirectional for performance. The Raw Bitmap is uploaded, its memory address is passed to the JNI bridge, "
        "and it is then horizontally sliced into 8 distinct segments. Each segment is processed by a dedicated PTA kernel instance. Once all "
        "threads have completed their tasks, the processed buffer is unlocked and returned to the Android UI for final rendering and PDF reporting."
    )
    add_image("optimization.png", "Detailed Data Flow and Optimization Strategy Diagram")

    # Chapter 5
    doc.add_page_break()
    add_heading("Chapter 5: System Design / Modules", 1)
    add_heading("5.1 Module 1: Desktop Compiler Pipeline", 2)
    add_paragraph(
        "Responsibility: This module is responsible for the transformation of PTA DSL into optimized hardware instructions. It includes a Lexer "
        "that uses a state-machine to tokenize raw text and an Emitter that generates valid ARM64 syntax. Implementation is done in C++17 to "
        "ensure fast compilation times. The final output is an assembly (.s) file that is immediately ready for deployment to the mobile application."
    )
    add_heading("5.2 Module 2: Android NDK & JNI Bridge", 2)
    add_paragraph(
        "Responsibility: This module handles the critical bridge between the high-level Android environment and the bare-metal assembly. It is "
        "responsible for querying hardware concurrency, slicing the image buffer into manageable chunks, and spawning native OS threads. By "
        "managing the memory lifecycle directly, it achieves zero-copy performance, ensuring that no time is wasted on unnecessary data duplication."
    )
    add_heading("5.3 Module 3: Performance Reporting Engine", 2)
    add_paragraph(
        "Responsibility: This module provides visual validation of the system's performance. It utilizes a background timer to poll kernel CPU "
        "frequency files (/sys/devices/system/cpu) and visualizes the core saturation in real-time. Finally, it uses the Android PDF Document "
        "API to generate formal technical reports, complete with grouped bar charts that contrast PTA performance against standard Java methods."
    )
    add_heading("5.4 User Interface Screenshots", 2)
    add_paragraph(
        "The following screenshots demonstrate the professional-grade benchmarking UI, featuring the live CPU core monitor, image comparison "
        "views, and the interactive performance reporting dialogs."
    )
    add_image("a.jpeg", "Main Benchmark UI with Image Comparison and CPU Core Monitor")
    add_image("b.jpeg", "Filter Selection and Parallel Processing Controls")
    add_image("c.jpeg", "Real-time Performance Metrics and Comparison Dashboard")

    # Chapter 6
    doc.add_page_break()
    add_heading("Chapter 6: Team Contribution", 1)
    add_paragraph(
        "The project was divided into four distinct architectural roles to mirror a production compiler team. Member 1 (Front-End) designed the "
        "state-machine Lexer and the responsive Android UI. Member 2 (Middle-End) engineered the Safety-Stripping Optimizer and the complex JNI "
        "bridge logic. Member 3 (Back-End) was responsible for physical Register Mapping and the multi-threaded execution architecture. Member 4 "
        "(Generator) developed the Assembly Emitter and the programmatic PDF reporting engine with dynamic graph generation."
    )

    # Chapter 7
    doc.add_page_break()
    add_heading("Chapter 7: Limitations", 1)
    add_paragraph(
        "While highly optimized, the current system has technical boundaries. It is strictly limited to ARM64 (AArch64) architectures, meaning "
        "it will not run on older 32-bit devices or x86 emulators. Furthermore, the PTA language currently lacks support for recursive function "
        "calls and high-level data structures like trees or linked lists, focusing exclusively on high-throughput linear array (bitmap) manipulation."
    )

    # Chapter 8
    doc.add_page_break()
    add_heading("Chapter 8: Enhancements / Future Work", 1)
    add_paragraph(
        "Future scalability ideas include the implementation of a JIT (Just-In-Time) compiler engine that would allow users to write and execute "
        "PTA code directly on their mobile device without a desktop build step. We also plan to integrate Vulkan compute shader support, "
        "allowing the PTA compiler to target mobile GPUs for massive parallel workloads that exceed the capabilities of even the fastest CPU cores."
    )

    # Chapter 9
    doc.add_page_break()
    add_heading("Chapter 9: Conclusion", 1)
    add_paragraph(
        "The PTA Compiler project successfully demonstrates the power of hardware-aware software design. By creating a custom DSL and a specialized "
        "optimization pipeline, we achieved performance gains that are impossible using standard high-level mobile development tools. This project "
        "validates the methodology of safety-stripping and multi-core hardware saturation, providing a clear real-world impact for the future of "
        "computational photography and high-performance mobile computing."
    )

    add_heading("References", 1)
    add_paragraph("1. ARM Architecture Reference Manual (ARMv8-A), 2024 Edition.")
    add_paragraph("2. Google Android NDK Guide - Best Practices for Native Multi-threading.")
    add_paragraph("3. Snapdragon 8 Gen 4 Oryon Core Optimization Whitepaper (Qualcomm).")
    add_paragraph("4. Advanced Image Processing in C++: SIMD and Parallelism Patterns.")

    doc.save("PTA_Compiler_Final_Project.docx")

if __name__ == "__main__":
    create_pta_doc_final()
