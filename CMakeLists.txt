cmake_minimum_required(VERSION 3.14)

set (TARGET_NAME lv_rlottie)

add_library( ${TARGET_NAME} STATIC )

target_include_directories(
    ${TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_include_directories(${TARGET_NAME}
    PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/../rlottie_samsung/rlottie/inc"
    )

target_sources(
    ${TARGET_NAME}
    PRIVATE
    "lv_rlottie.c"
)

target_link_libraries(
    ${TARGET_NAME}
    PUBLIC
    lvgl_lib
    rlottie
)