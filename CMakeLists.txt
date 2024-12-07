cmake_minimum_required(VERSION 3.14)

# This also doesn't seem to be necessary
# # Add these lines BEFORE the project() call
# set(CMAKE_SYSTEM_NAME Emscripten)
# set(CMAKE_SYSTEM_VERSION 1)
# set(EMSCRIPTEN TRUE)


project(silent-letters)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)


# START EMSCRIPTEN ADD

    # Ensure we're using Emscripten compilers
    set(CMAKE_C_COMPILER emcc)
    set(CMAKE_CXX_COMPILER em++)

    # Specify the Emscripten toolchain file BEFORE the project() call
    set(CMAKE_TOOLCHAIN_FILE "/usr/local/Cellar/emscripten/3.1.61/libexec/cmake/Modules/Platform/Emscripten.cmake")

    add_executable(silent-letters src/main.cpp src/CMU_Dict.cpp src/silent_letters.cpp)

    # Include directories
    target_include_directories(silent-letters PUBLIC ${CMAKE_SOURCE_DIR}/include)

    # Add Emscripten-specific flags to output WASM
    set_target_properties(silent-letters PROPERTIES
        LINK_FLAGS "-lembind -s INVOKE_RUN=0 -s MODULARIZE=1 --preload-file ${CMAKE_SOURCE_DIR}/data/cmudict-0.7b@/data/cmudict-0.7b -s EXPORTED_RUNTIME_METHODS='[\"ccall\", \"cwrap\"]'"
    )

# END EMPSCRIPTEN ADD

# START NON-EMSCRIPTEN BUILD

# FetchContent setup to download Catch2
# include(FetchContent)

# FetchContent_Declare(
#   Catch2
#   GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#   GIT_TAG        v3.7.1 
# )

# FetchContent_MakeAvailable(Catch2)

# Main application
# add_subdirectory(src)

# Enable testing
# enable_testing()

# Test directory
# add_subdirectory(tests)

# END NON-EMSCRIPTEN BUILD
