include(hello_imgui_add_app)
set(implot_dir ${CMAKE_CURRENT_LIST_DIR}/../external/implot)
hello_imgui_add_app(implot_demo
    demo.cpp
    ${implot_dir}/implot.h
    ${implot_dir}/implot.cpp
    ${implot_dir}/implot_demo.cpp
    )
target_include_directories(implot_demo PRIVATE ${implot_dir})
