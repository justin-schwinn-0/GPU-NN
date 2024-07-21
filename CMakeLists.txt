cmake_minimum_required(VERSION 3.22.1)

project(GPU-NN)

set(Sources
    gpuNN.cpp
    CsvReader.cpp
    DataFrame.cpp
    NeuralNet.cpp
)

set(Headers
    CsvReader.h
    DataFrame.h
    Logger.h
    Utils.h
    NeuralNet.h
    Layer.h
)

add_executable(${PROJECT_NAME} ${Sources} ${Headers})
