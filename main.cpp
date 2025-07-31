#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>
#include <limits>
#include <filesystem>

// Define M_PI if it's not already defined by your compiler/standard library
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ONNX Runtime includes
#include <onnxruntime_cxx_api.h>

// Function to calculate time features (if you need to do this in C++ for new data)
// This should be consistent with how you generated Time_sin and Time_cos in Python
void calculateTimeFeatures(int hour, int minute, float& time_sin, float& time_cos) {
    int total_minutes_from_midnight = hour * 60 + minute;
    double minutes_in_day = 24.0 * 60.0;
    time_sin = static_cast<float>(std::sin(2 * M_PI * total_minutes_from_midnight / minutes_in_day));
    time_cos = static_cast<float>(std::cos(2 * M_PI * total_minutes_from_midnight / minutes_in_day));
}

int main() {
    // 1. Model Path
    std::string model_path_str = "battery_charge_predictor.onnx";

    // --- Add a file existence check ---
    if (!std::filesystem::exists(model_path_str)) {
        std::cerr << "Error: Model file not found at: " << model_path_str << std::endl;
        std::cerr << "Please ensure 'battery_charge_predictor.onnx' is in the same directory as the executable." << std::endl;
        return 1; // Exit with an error code
    }

    // Convert std::string to std::wstring for Windows compatibility with ONNX Runtime
    std::wstring model_path_wstr(model_path_str.begin(), model_path_str.end());
    const wchar_t* model_path_w = model_path_wstr.c_str();


    // 2. ONNX Runtime Environment Setup
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "BatteryPredictor"); // Can change back to WARNING now

    // Configure session options (optional, but good practice)
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);
    session_options.SetGraphOptimizationLevel(ORT_ENABLE_EXTENDED);

    // 3. Load the Model
    std::cout << "Loading model from: " << model_path_str << std::endl;
    Ort::Session session(env, model_path_w, session_options); // Use wchar_t* for path
    std::cout << "Model loaded successfully." << std::endl;

    // Get the default allocator
    Ort::AllocatorWithDefaultOptions allocator;

    // 4. Get Input and Output Node Names
    size_t num_input_nodes = session.GetInputCount();
    size_t num_output_nodes = session.GetOutputCount();

    if (num_input_nodes != 1 || num_output_nodes != 1) {
        std::cerr << "Error: Expected 1 input and 1 output node, but found "
                  << num_input_nodes << " inputs and " << num_output_nodes << " outputs." << std::endl;
        return 1;
    }

    Ort::AllocatedStringPtr input_name_ptr = session.GetInputNameAllocated(0, allocator);
    Ort::AllocatedStringPtr output_name_ptr = session.GetOutputNameAllocated(0, allocator);

    std::vector<const char*> input_node_names_chars = { input_name_ptr.get() };
    std::vector<const char*> output_node_names_chars = { output_name_ptr.get() };

    std::cout << "Input Node 0: Name=" << input_node_names_chars[0] << std::endl;
    std::cout << "Output Node 0: Name=" << output_node_names_chars[0] << std::endl;


    // 5. Prepare Input Data for a single prediction
    // Store original input values for user-friendly display
    float battery_level_input = 0.55f;         // e.g., 55%
    float battery_temperature_input = 28.0f;   // e.g., 28.0 Celsius
    float battery_current_input = -30.0f;     // e.g., -30 Amperes (discharging)
    int input_hour = 14;                      // For display
    int input_minute = 30;                    // For display

    // Calculate time features for the model
    float time_sin_val, time_cos_val;
    calculateTimeFeatures(input_hour, input_minute, time_sin_val, time_cos_val);

    // Combine all features into a single input vector for the ONNX model
    std::vector<float> input_tensor_values = {
        battery_level_input,
        battery_temperature_input,
        time_sin_val,
        time_cos_val,
        battery_current_input
    };

    // 6. Create Input Tensor
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    std::vector<int64_t> input_shape = {1, (int64_t)input_tensor_values.size()};

    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        memory_info,
        input_tensor_values.data(),
        input_tensor_values.size(),
        input_shape.data(),
        input_shape.size()
    );
    std::vector<Ort::Value> input_tensors_vec;
    input_tensors_vec.push_back(std::move(input_tensor));

    // 7. Run Inference
    std::cout << "\nRunning inference..." << std::endl;
    auto output_tensors = session.Run(
        Ort::RunOptions{nullptr},
        input_node_names_chars.data(),
        input_tensors_vec.data(),
        input_tensors_vec.size(),
        output_node_names_chars.data(),
        output_node_names_chars.size()
    );

    // 8. Process Output
    if (output_tensors.empty()) {
        std::cerr << "Error: Model returned no outputs." << std::endl;
        return 1;
    }

    float* predicted_time_ptr = output_tensors[0].GetTensorMutableData<float>();
    float predicted_minutes_float = predicted_time_ptr[0];

    // --- User-friendly output conversion ---
    int predicted_total_minutes = static_cast<int>(std::round(predicted_minutes_float)); // Round to nearest minute
    int predicted_hours = predicted_total_minutes / 60;
    int predicted_remaining_minutes = predicted_total_minutes % 60;

    std::cout << "\n--- Battery Charge Prediction ---" << std::endl;
    std::cout << "Based on the following conditions:" << std::endl;
    std::cout << "  - Battery Level: " << static_cast<int>(battery_level_input * 100) << "%" << std::endl;
    std::cout << "  - Battery Temperature: " << battery_temperature_input << "°C" << std::endl;
    std::cout << "  - Current Time: " << (input_hour < 10 ? "0" : "") << input_hour << ":"
                                       << (input_minute < 10 ? "0" : "") << input_minute << std::endl;
    std::cout << "  - Battery Current: " << battery_current_input << " Amperes (negative means discharging)" << std::endl;
    std::cout << "\nPredicted Time to Next Charge: ";
    if (predicted_hours > 0) {
        std::cout << predicted_hours << " hour(s) and ";
    }
    std::cout << predicted_remaining_minutes << " minute(s)" << std::endl;
    std::cout << "(Raw prediction: " << predicted_minutes_float << " minutes)" << std::endl;

    return 0;
}