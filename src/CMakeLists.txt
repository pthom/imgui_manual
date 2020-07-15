include(hello_imgui_add_app)

execute_process(COMMAND bash -c ${CMAKE_CURRENT_LIST_DIR}/populate_assets.sh)

set(textedit_dir ${CMAKE_CURRENT_LIST_DIR}/../external/ImGuiColorTextEdit)
set(fplus_dir ${CMAKE_CURRENT_LIST_DIR}/../external/FunctionalPlus/include)
set(imgui_markdown_dir ${CMAKE_CURRENT_LIST_DIR}/../external/imgui_markdown)

add_subdirectory(imgui_utilities)
add_subdirectory(source_parse)

FILE(GLOB sources_imgui_manual
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    )
hello_imgui_add_app(imgui_manual ${sources_imgui_manual})
target_link_libraries(imgui_manual PRIVATE imgui_utilities source_parse)

if (IMGUI_MANUAL_CAN_WRITE_IMGUI_DEMO_CPP)
    target_compile_definitions(imgui_manual
        PRIVATE
        IMGUI_MANUAL_CAN_WRITE_IMGUI_DEMO_CPP
        -DIMGUI_MANUAL_REPO_DIR="${CMAKE_SOURCE_DIR}"
        )
endif()
