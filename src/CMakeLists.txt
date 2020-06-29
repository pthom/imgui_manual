include(hello_imgui_add_app)

execute_process(COMMAND bash -c ${CMAKE_CURRENT_LIST_DIR}/populate_assets.sh)

set(implot_dir ${CMAKE_CURRENT_LIST_DIR}/../external/implot)
set(textedit_dir ${CMAKE_CURRENT_LIST_DIR}/../external/ImGuiColorTextEdit)
hello_imgui_add_app(implot_demo
    demo.cpp
    ${implot_dir}/implot.h
    ${implot_dir}/implot.cpp
    ${implot_dir}/implot_demo.cpp
    ${textedit_dir}/TextEditor.h
    ${textedit_dir}/TextEditor.cpp
    )
target_include_directories(implot_demo PRIVATE ${implot_dir} ${textedit_dir})
