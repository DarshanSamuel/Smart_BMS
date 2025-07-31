cmake_minimum_required(VERSION 3.10)
project(OnnxBatteryPredictor CXX)

set(CMAKE_CXX_STANDARD 17) # Ensures C++17 features are available

# Set this to the absolute path where you extracted the ONNX Runtime C++ package.
# Example for Windows: set(ONNX_RUNTIME_DIR "C:/path/to/onnxruntime-win-x64-1.17.1")
# Example for Linux:   set(ONNX_RUNTIME_DIR "/home/user/onnxruntime-linux-x64-1.17.1")
set(ONNX_RUNTIME_DIR "C:/dev/onnxruntime-win-x64-1.22.1") # <--- REMEMBER TO CHANGE THIS!

# Check if the ONNX_RUNTIME_DIR exists
if (NOT EXISTS "${ONNX_RUNTIME_DIR}")
    message(FATAL_ERROR "ONNX_RUNTIME_DIR not found: ${ONNX_RUNTIME_DIR}. Please update the path in CMakeLists.txt.")
endif()
if (NOT EXISTS "${ONNX_RUNTIME_DIR}/include")
    message(FATAL_ERROR "ONNX_RUNTIME_DIR/include not found: ${ONNX_RUNTIME_DIR}/include. Please check the ONNX_RUNTIME_DIR path and its contents.")
endif()
if (NOT EXISTS "${ONNX_RUNTIME_DIR}/lib")
    message(FATAL_ERROR "ONNX_RUNTIME_DIR/lib not found: ${ONNX_RUNTIME_DIR}/lib. Please check the ONNX_RUNTIME_DIR path and its contents.")
endif()


# Add include directories so the compiler can find ONNX Runtime headers
include_directories(${ONNX_RUNTIME_DIR}/include)

# Add library directories so the linker can find ONNX Runtime libraries
link_directories(${ONNX_RUNTIME_DIR}/lib)

# Add your source file (assuming it's named main.cpp)
add_executable(SmartBMS main.cpp)

# Link against the ONNX Runtime library
# Note: The exact library name might vary slightly (e.g., onnxruntime.lib on Windows)
# but CMake usually handles this if it finds the .lib or .so file in link_directories.
target_link_libraries(SmartBMS onnxruntime)

# --- Optional: Post-build steps for Windows (copying DLLs) ---
# This is crucial for Windows so the ONNX Runtime DLL is present next to your executable.
if(WIN32)
    # Define the source DLL path based on the chosen build config (Debug/Release)
    # This assumes a typical ONNX Runtime build structure. Adjust if yours is different.
    set(ONNX_RUNTIME_DLL_PATH "${ONNX_RUNTIME_DIR}/lib/onnxruntime.dll")

    # Add a custom command to copy the DLL after the executable is built
    add_custom_command(TARGET SmartBMS POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${ONNX_RUNTIME_DLL_PATH}"
                "$<TARGET_FILE_DIR:SmartBMS>/"
        COMMENT "Copying ONNX Runtime DLL to build directory: $<TARGET_FILE_DIR:SmartBMS>/"
    )
    message(STATUS "Configured to copy ONNX Runtime DLL for Windows.")
endif()

message(STATUS "CMake configuration complete for ONNX Battery Predictor.")