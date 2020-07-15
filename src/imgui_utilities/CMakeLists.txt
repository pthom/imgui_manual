FILE(GLOB sources
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${imgui_markdown_dir}/imgui_markdown.h
    ${textedit_dir}/TextEditor.h
    ${textedit_dir}/TextEditor.cpp
    )
add_library(imgui_utilities STATIC ${sources})
target_include_directories(imgui_utilities PUBLIC
    ${textedit_dir}
    ${fplus_dir}
    ${imgui_markdown_dir}
    )
target_link_libraries(imgui_utilities PRIVATE hello_imgui)
