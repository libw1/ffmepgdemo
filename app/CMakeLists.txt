# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

#set(src_DIR ${CMAKE_SOURCE_DIR})
set(ffmpeg_include_DIR ${CMAKE_SOURCE_DIR}/src/main/cpp/ffmpeg/include)
set(ffmpeg_lib_DIR ${CMAKE_SOURCE_DIR}/src/main/cpp/ffmpeg/lib/${CMAKE_ANDROID_ARCH_ABI})

add_library(libavformat STATIC IMPORTED)
set_target_properties(libavformat PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_DIR}/libavformat.a)

add_library(libavcodec STATIC IMPORTED)
set_target_properties(libavcodec PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_DIR}/libavcodec.a)

add_library(libswresample STATIC IMPORTED)
set_target_properties(libswresample PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_DIR}/libswresample.a)

add_library(libswscale STATIC IMPORTED)
set_target_properties(libswscale PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_DIR}/libswscale.a)

add_library(libavutil STATIC IMPORTED)
set_target_properties(libavutil PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_DIR}/libavutil.a)

add_library(libavfilter STATIC IMPORTED)
set_target_properties(libavfilter PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_DIR}/libavfilter.a)
add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp
             src/main/cpp/audio_player.c
             src/main/cpp/media_player.c
             src/main/cpp/AVpacket_queue.c
             src/main/cpp/openSL_audio_player)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       libavformat
                       libavcodec
                       libswresample
                       libswscale
                       libavutil
                       libavfilter
                       -landroid
                       -lz
                       -lOpenSLES #openSLES
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )