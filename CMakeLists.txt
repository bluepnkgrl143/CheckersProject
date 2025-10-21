cmake_minimum_required(VERSION 3.10)
project(CheckersProject C)

set(CMAKE_C_STANDARD 11)

# Make sure you have SDL3 downloaded and extracted to C: (recommendation)
# Set SDL3 path (adjust this to your actual SDL3 location)
set(SDL3_DIR "C:/SDL3/SDL3-3.2.24")

# Include SDL3 headers
include_directories(${SDL3_DIR}/include)

# Link library directory
link_directories(${SDL3_DIR}/lib/x64)

add_executable(CheckersProject
        BitBoardGE.h
        BitBoardGE.c
        main.c
)

# Link SDL3 library
target_link_libraries(CheckersProject SDL3)

# Copy SDL3.dll to build directory after building
add_custom_command(TARGET CheckersProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_DIR}/lib/x64/SDL3.dll"
        $<TARGET_FILE_DIR:CheckersProject>
        COMMENT "Copying SDL3.dll to output directory"
)



