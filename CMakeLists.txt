cmake_minimum_required(VERSION 3.31)
project(dao)

set(CMAKE_CXX_STANDARD 23)

add_library(dao
        code/src/core/basic_drawing_elements/atlas_texture.cpp
        code/src/core/frame/app.cpp
        code/src/core/frame/vertex_batch_builder.cpp
        code/src/core/frame/window.cpp
        code/src/core/tool/bounding_box.cpp
        code/src/core/tool/mapped_region.cpp
        code/src/core/frame/window_command.cpp
)
set_target_properties(dao PROPERTIES DEBUG_POSTFIX "_d")

target_include_directories(dao
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/code/include>
        $<INSTALL_INTERFACE:include>
)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(utf8cpp CONFIG REQUIRED)

target_link_libraries(dao
        PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        utf8::cpp
        utf8cpp::utf8cpp
)

install(TARGETS dao
        EXPORT daoTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

install(DIRECTORY code/include/ DESTINATION include)

install(EXPORT daoTargets
        FILE daoTargets.cmake
        NAMESPACE dao::
        DESTINATION lib/cmake/dao
)

install(FILES daoConfig.cmake DESTINATION lib/cmake/dao)