cmake_minimum_required(VERSION 3.31)

project(example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(dao REQUIRED PATHS "../build/install")

add_executable(${PROJECT_NAME}
        assets/textures/config/atlas_region.cpp
        main.cpp
        pages/hello_dao_page/hello_dao_page.cpp
        tray/tray.cpp
        context/states/states.cpp
        context/services/service.cpp
)

# 本地 include
target_include_directories(${PROJECT_NAME} PRIVATE
        assets/textures/inc
        pages
        tray
        context
)

# 链接 dao
target_link_libraries(${PROJECT_NAME} PRIVATE dao::dao)

# 先把所有需要的目录全部建好
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/assets/ttf"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/assets/textures"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/assets/textures/atlas"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets/ttf"
        "${CMAKE_BINARY_DIR}/assets/ttf"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets/textures/atlas"
        "${CMAKE_BINARY_DIR}/assets/textures/atlas"
)