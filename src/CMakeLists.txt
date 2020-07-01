include(hello_imgui_add_app)

execute_process(COMMAND bash -c ${CMAKE_CURRENT_LIST_DIR}/populate_assets.sh)

set(textedit_dir ${CMAKE_CURRENT_LIST_DIR}/../external/ImGuiColorTextEdit)
set(fplus_dir ${CMAKE_CURRENT_LIST_DIR}/../external/FunctionalPlus/include)
set(imgui_markdown_dir ${CMAKE_CURRENT_LIST_DIR}/../external/imgui_markdown)

FILE(GLOB sources
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utilities/*.h
    ${CMAKE_CURRENT_LIST_DIR}/utilities/*.cpp
    )
hello_imgui_add_app(imgui_manual
    ${sources}
    ${imgui_markdown_dir}/imgui_markdown.h
    ${textedit_dir}/TextEditor.h
    ${textedit_dir}/TextEditor.cpp
    )
target_include_directories(imgui_manual PRIVATE
    ${textedit_dir} ${fplus_dir} ${imgui_markdown_dir}
    ${CMAKE_CURRENT_LIST_DIR}
)

if (IMGUI_HELLODEMO_CAN_WRITE_IMGUI_DEMO_CPP)
    target_compile_definitions(imgui_manual
        PRIVATE
        IMGUI_HELLODEMO_CAN_WRITE_IMGUI_DEMO_CPP
        -DIMGUI_HELLODEMO_REPO_DIR="${CMAKE_SOURCE_DIR}"
        -DIMGUI_HELLODEMO_BIN_DIR="${CMAKE_CURRENT_BINARY_DIR}"
        )
endif()
