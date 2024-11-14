add_executable(silent-letters main.cpp CMU_Dict.cpp silent_letters.cpp)

# Include directories
target_include_directories(silent-letters PUBLIC ${CMAKE_SOURCE_DIR}/include)
