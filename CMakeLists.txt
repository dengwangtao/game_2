cmake_minimum_required(VERSION 3.10.0)
project(GhostEscape VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_BUILD_TYPE Debug)


# 设置编译选项
if (MSVC)
    add_compile_options(/W4)
    add_compile_options(/utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options(-Wno-unused-but-set-variable)
    add_compile_options(-Wno-unused-parameter)
    add_compile_options(-Wno-reorder)
endif()


# 设置输出目录
set(BIN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_OUTPUT_DIR})


find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(glm REQUIRED)

# =====================================================================================================
set(ASSETS_ZIP "${CMAKE_SOURCE_DIR}/assets.zip")
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

# 判断 assets 目录是否存在
if(NOT EXISTS "${ASSETS_DIR}")
    message(STATUS "Assets directory not found, extracting assets.zip...")

    # 创建目录
    file(MAKE_DIRECTORY "${ASSETS_DIR}")

    # 解压，根据平台选择命令
    if(WIN32)
        # 使用 PowerShell Expand-Archive
        execute_process(
            COMMAND powershell -Command "Expand-Archive -Path '${ASSETS_ZIP}' -DestinationPath '${ASSETS_DIR}'"
            RESULT_VARIABLE result
        )
    else()
        # Linux / macOS 使用 unzip
        execute_process(
            COMMAND unzip -o "${ASSETS_ZIP}" -d "${ASSETS_DIR}"
            RESULT_VARIABLE result
        )
    endif()

    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Failed to extract assets.zip to assets directory")
    endif()
else()
    message(STATUS "Assets directory already exists, skipping extraction")
endif()

# =====================================================================================================

add_subdirectory(src)
# add_subdirectory(demo)