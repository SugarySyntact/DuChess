add_executable(duchess main.cpp)

set_target_properties(duchess
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)