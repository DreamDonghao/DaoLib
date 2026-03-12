if(NOT CMAKE_PROJECT_NAME)
    cmake_minimum_required(VERSION 3.31)
    project(test LANGUAGES CXX)
    # 独立构建时查找已安装的 dao
    find_package(dao REQUIRED PATHS "../build/install")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
        assets/textures/config/atlas_region.cpp
        main.cpp
        pages/hello_dao_page.cpp
        pages/page_a/page_a.cpp
        tray/AppTray.cpp
)

# 本地 include
target_include_directories(${PROJECT_NAME} PRIVATE
        assets/textures/inc
        pages
        tray
)

# 链接 dao：作为子项目直接链接目标，独立构建时使用导入目标
if(TARGET dao)
    target_link_libraries(${PROJECT_NAME} PRIVATE dao)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE dao::dao)
endif()

# 先把所有需要的目录全部建好
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/assets/ttf"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/assets/textures"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/assets/textures/atlas"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/test/assets/ttf"
        "${CMAKE_BINARY_DIR}/test/assets/ttf"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/test/assets/textures/atlas"
        "${CMAKE_BINARY_DIR}/test/assets/textures/atlas"
)