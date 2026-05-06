from docx import Document
from docx.shared import Pt, Inches
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml.ns import qn

def create_pta_doc():
    doc = Document()

    # Define style helper
    def set_font(run, size, bold=False):
        run.font.name = 'Times New Roman'
        run._element.rPr.rFonts.set(qn('w:ascii'), 'Times New Roman')
        run.font.size = Pt(size)
        run.font.bold = bold

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

    # --- 5. Abstract ---
    add_heading("Abstract", 1)
    abstract_text = (
        "This project presents the design and implementation of the Portable Typed Assembly (PTA) Compiler, "
        "a high-performance Domain Specific Language (DSL) engineered for ultra-fast image processing on Android devices. "
        "The problem addressed is the performance bottleneck in standard Java/Kotlin image processing, which often "
        "leaves mobile hardware underutilized. Our solution introduces a custom assembly-like language that is "
        "compiled into optimized ARM64 assembly code. Key features include a 'Safety-Stripping' optimizer that "
        "replaces software-level overflow checks with hardware SIMD saturation instructions (NEON). "
        "Built using C++17 for the desktop compiler and Kotlin with the Android NDK for the mobile frontend, "
        "the system demonstrates up to an 800% speed increase on Snapdragon Oryon cores compared to standard Java "
        "implementations. The outcome is a scalable architecture that bridges low-level hardware control with "
        "high-level mobile application ecosystems."
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
        "The Portable Typed Assembly (PTA) Compiler is a specialized development tool designed to maximize "
        "computational efficiency in mobile image processing. By abstracting complex hardware instructions "
        "into a readable assembly-like syntax, PTA empowers developers to write high-performance kernels."
    )
    add_heading("1.2 Background / Motivation", 2)
    add_paragraph(
        "Modern mobile processors, such as the Snapdragon 8 Gen 4, possess immense parallel processing power. "
        "However, high-level languages like Java and Kotlin often fail to saturate these hardware capabilities "
        "due to the overhead of the Android Runtime (ART) and the safety checks imposed by managed code."
    )
    add_heading("1.3 Problem Statement", 2)
    add_paragraph(
        "Standard image processing libraries on Android are frequently limited to single-threaded execution "
        "or sub-optimal SIMD utilization. This results in significant latency when processing high-resolution "
        "images, leading to a poor user experience in real-time photography and video editing applications."
    )
    add_heading("1.4 Objectives & Methodology", 2)
    add_paragraph(
        "The primary objective is to create a compiler that translates PTA DSL into optimized ARM64 assembly. "
        "The methodology involves a four-stage pipeline: lexical analysis, safety-stripping optimization, "
        "register mapping, and final assembly emission, followed by multi-threaded integration on Android."
    )
    add_heading("1.5 Project Organization", 2)
    add_paragraph(
        "The document is organized into nine chapters, detailing everything from system architecture "
        "to implementation results, team roles, and future scalability plans."
    )

    # Chapter 2
    doc.add_page_break()
    add_heading("Chapter 2: Proposed System", 1)
    add_heading("2.1 System Overview", 2)
    add_paragraph(
        "The proposed PTA system consists of a cross-compiler that generates bare-metal ARM64 code. "
        "This code is packaged as a native library within an Android application, allowing for direct "
        "interaction with the CPU's NEON vector engine."
    )
    add_heading("2.2 System Workflow", 2)
    add_paragraph(
        "The workflow begins with the user writing a PTA script. The compiler analyzes the logic, "
        "strips redundant safety checks, and maps variables to hardware registers. The resulting "
        "assembly is then linked via a JNI bridge to the Android UI."
    )
    add_heading("2.3 Key Features / Innovations", 2)
    add_paragraph(
        "The system's core innovation is 'Safety-Stripping.' By trusting hardware-level saturating "
        "arithmetic (uqadd), we eliminate thousands of software branch instructions per frame."
    )

    # Chapter 3
    doc.add_page_break()
    add_heading("Chapter 3: Hardware & Software Requirements", 1)
    add_heading("3.1 Software Requirements", 2)
    add_paragraph("C++17 Compiler (GCC/Clang), Android Studio Jellyfish+, CMake 3.16+, Android NDK r25+.")
    add_heading("3.2 Hardware Requirements", 2)
    add_paragraph("ARM64 based Android device (Snapdragon 8 Gen 4 or equivalent recommended for Oryon core performance).")
    add_heading("3.3 Security / Reliability", 2)
    add_paragraph(
        "The system ensures reliability through strict register mapping and buffer boundary validation "
        "at the JNI layer, preventing common native memory corruption issues."
    )

    # Chapter 4
    doc.add_page_break()
    add_heading("Chapter 4: System Architecture", 1)
    add_heading("4.1 High-Level Architecture", 2)
    add_paragraph(
        "The architecture follows a modular design: a C++ Desktop Compiler for offline code generation "
        "and a Kotlin-based Android App for real-time benchmark execution."
    )
    add_heading("4.2 Data Flow / Diagrams", 2)
    add_paragraph(
        "Data flows from the Android Bitmap into a native C++ buffer, where it is horizontally sliced "
        "and processed by parallel PTA assembly kernels across multiple CPU cores."
    )

    # Chapter 5
    doc.add_page_break()
    add_heading("Chapter 5: System Design / Modules", 1)
    add_heading("5.1 Module 1: Desktop Compiler Pipeline", 2)
    add_paragraph(
        "Responsibility: To convert .tasm source files into optimized ARM64 .s assembly. "
        "Design: A state-machine based Lexer followed by a pattern-matching Optimizer. "
        "Implementation: C++17 with STL containers. Output: Assembly source code."
    )
    add_heading("5.2 Module 2: Android NDK & JNI Bridge", 2)
    add_paragraph(
        "Responsibility: Managing native memory and multi-core thread spawning. "
        "Design: Synchronized C++ threads using hardware_concurrency(). "
        "Implementation: NDK C++ and JNI headers. Output: High-speed processed image buffer."
    )
    add_heading("5.3 Module 3: Performance Reporting Engine", 2)
    add_paragraph(
        "Responsibility: Visualizing speedup results and hardware stats. "
        "Design: Kotlin-based UI with PDF document generator. "
        "Implementation: android.graphics.pdf.PdfDocument. Output: PDF Report."
    )

    # Chapter 6
    doc.add_page_break()
    add_heading("Chapter 6: Team Contribution", 1)
    add_paragraph(
        "Member 1 (Front-End): Responsible for the Lexer and Android UI design. "
        "Member 2 (Middle-End): Developed the Safety-Stripping Optimizer and JNI logic. "
        "Member 3 (Back-End): Managed Register Mapping and Multi-threading. "
        "Member 4 (Generator): Handled Assembly Emission and PDF Graphing."
    )

    # Chapter 7
    doc.add_page_break()
    add_heading("Chapter 7: Limitations", 1)
    add_paragraph(
        "Current constraints include support only for ARM64 architectures. "
        "The compiler does not currently support complex control flow structures like while loops "
        "or function calls within the PTA script."
    )

    # Chapter 8
    doc.add_page_break()
    add_heading("Chapter 8: Enhancements / Future Work", 1)
    add_paragraph(
        "Future enhancements include a Just-In-Time (JIT) compiler to allow on-device code "
        "compilation and support for Vulkan compute shaders for even higher GPU-based parallelism."
    )

    # Chapter 9
    doc.add_page_break()
    add_heading("Chapter 9: Conclusion", 1)
    add_paragraph(
        "In conclusion, the PTA Compiler successfully demonstrates that domain-specific low-level "
        "optimizations can provide significant performance gains in mobile computing. By bridging "
        "the gap between high-level development and bare-metal execution, we have created a tool "
        "capable of meeting the demands of modern image processing."
    )

    add_heading("References", 1)
    add_paragraph("1. ARM Architecture Reference Manual (ARMv8-A).")
    add_paragraph("2. Android NDK Documentation - Google Developers.")
    add_paragraph("3. Modern C++ Design: Generic Programming and Design Patterns Applied.")

    doc.save("PTA_Compiler_Project_Detailed.docx")

if __name__ == "__main__":
    create_pta_doc()
