include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/v1.16.0.zip
)
# For Windows (ewww)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(duchess-tests
    types_test.cpp
    bitboard_test.cpp
    zobrist_test.cpp
    position_test.cpp
)

target_link_libraries(duchess-tests
    PRIVATE
        duchess
        gtest_main
        gmock_main
)

target_include_directories(duchess-tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

include(GoogleTest)
gtest_discover_tests(duchess-tests)