

message("构建 ${PROJECT_NAME}... CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


# 设置目标名称
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# 扫描所有cpp
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS *.cpp *.h)

add_executable(${TARGET} ${SRCS})


if (MSVC)
    target_compile_options(${TARGET} PRIVATE /wd4100 /wd4244 /wd4267)
endif()


target_include_directories(${TARGET} PRIVATE
    ${CMAKE_SOURCE_DIR}/thirdparty/spdlog
)

target_compile_definitions(${TARGET} PRIVATE
    # SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG
)

target_link_libraries(
    ${TARGET}
        ${SDL3_LIBRARIES} # 在sdl3-config.cmake中定义的变量
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
        glm::glm
)