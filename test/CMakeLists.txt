project(test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
        assets/textures/config/atlas_region.cpp
        main.cpp
        pages/TextTestPage/TextTestPage.cpp
        pages/TextTestPage/TextTestPage.hpp
        pages/CellularAutomataPage/CellularAutomataPage.cpp
        pages/CellularAutomataPage/CellularAutomataPage.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        assets/textures/inc
        pages
)

target_link_libraries(${PROJECT_NAME} PRIVATE dao)

# 复制资产到构建目录
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/test/assets/ttf"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/test/assets/textures"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/test/assets/textures/atlas"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/test/assets/ttf"
        "${CMAKE_BINARY_DIR}/test/assets/ttf"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/test/assets/textures/atlas"
        "${CMAKE_BINARY_DIR}/test/assets/textures/atlas"
)