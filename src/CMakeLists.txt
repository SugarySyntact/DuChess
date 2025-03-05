add_library(duchess
    types.cpp
    bitboard.cpp
)

target_include_directories(duchess
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(duchess-app main.cpp)

target_link_libraries(duchess-app PRIVATE duchess)