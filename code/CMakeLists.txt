target_include_directories(dao
        include
)

add_library(dao
        src/core/basic_drawing_elements/atlas_texture.cpp
        src/core/frame/app.cpp
        src/core/frame/vertex_batch_builder.cpp
        src/core/frame/window.cpp
        src/core/tool/bounding_box.cpp
        src/core/tool/mapped_region.cpp
        src/core/frame/window_command.cpp
)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(utf8cpp CONFIG REQUIRED)
target_link_libraries(dao PRIVATE SDL3::SDL3 PRIVATE SDL3_image::SDL3_image PRIVATE SDL3_ttf::SDL3_ttf PRIVATE utf8::cpp utf8cpp::utf8cpp)

