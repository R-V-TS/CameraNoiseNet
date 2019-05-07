# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


set(CMAKE_VERBOSE_MAKEFILE on)
set(COMMON_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../../../common)

file( GLOB app_src_files
        "src/main/cpp/*.cpp"
        "src/main/cpp/imageProccesing.cpp")


add_library(ndk_camera SHARED ${app_src_files})

set(external_lib "${CMAKE_SOURCE_DIR}/external_lib")

add_library(tensorflowLite SHARED IMPORTED)
set_target_properties(tensorflowLite PROPERTIES IMPORTED_LOCATION ${external_lib}/libs/${ANDROID_ABI}/libtensorflowlite.so)

target_include_directories(ndk_camera PRIVATE ${external_lib}/include)
target_include_directories(ndk_camera PRIVATE ${external_lib}/include/flatbuffers/include)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror")

target_link_libraries(ndk_camera
        android
        log
        camera2ndk
        mediandk
        tensorflowLite
        )
