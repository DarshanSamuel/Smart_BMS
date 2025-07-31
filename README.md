## Smart BMS Battery Charge Predictor
# Project Overview
This repository presents a complete machine learning solution for a Smart Battery Management System (BMS). The project aims to predict the remaining time until a battery needs its next charge, leveraging real-time sensor data. It demonstrates a seamless workflow from a Python-based model training pipeline to a high-performance C++ deployment using ONNX.

The solution is divided into two main parts:

Python Script: Handles data cleaning, feature engineering, training a RandomForestRegressor model with scikit-learn, and exporting the final model to the ONNX format.

C++ Application: A lightweight and efficient C++ program that uses the ONNX Runtime library to load the exported model and perform real-time inference on new data.

This setup is ideal for integrating machine learning capabilities into resource-constrained environments like embedded systems, where Python may not be feasible.

# Prerequisites
To replicate this project, you will need to have the following installed:

# Python Environment
Python 3.8+

pip install pandas numpy scikit-learn onnx onnxmltools skl2onnx

# C++ Environment
A C++ compiler (e.g., GCC, Clang, MSVC)

# CMake 3.10+

The ONNX Runtime C++ library (pre-built binaries are recommended)

# Usage
1. Model Training & Export
Ensure your cleaned_battery_data_with_target.csv file is present.

Run the provided Python script to train the model and export it to battery_charge_predictor.onnx.

2. C++ Application
Place the battery_charge_predictor.onnx file and the main.cpp file in your C++ project directory.

Update the ONNX_RUNTIME_DIR variable in the CMakeLists.txt file to point to your ONNX Runtime installation.

Build the project using CMake to generate the executable.

Run the executable to see a sample prediction.

This project offers a robust foundation for integrating predictive maintenance and intelligent decision-making into your battery management systems.
