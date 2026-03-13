set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test
        assets/textures/config/atlas_region.cpp
        main.cpp
        pages/hello_dao_page.cpp
        pages/page_a/page_a.cpp
        tray/AppTray.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        assets/textures/inc
        pages
        tray
)

target_link_libraries(${PROJECT_NAME} PRIVATE dao)

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