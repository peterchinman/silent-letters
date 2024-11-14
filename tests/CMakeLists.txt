# Add the test executable
add_executable(tests test_silent_letters.cpp ${CMAKE_SOURCE_DIR}/src/CMU_Dict.cpp ${CMAKE_SOURCE_DIR}/src/silent_letters.cpp)

# Link the test executable with Catch2
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# Include directories for the test files
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Register tests with CTest
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)
