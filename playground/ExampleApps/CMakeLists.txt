# make_example_apps will populate the folder playground/ExampleApps
# with the example apps from imgui_demo.cpp
set(make_example_apps_exe ${CMAKE_BINARY_DIR}/src/make_example_apps/make_example_apps)
if (EXISTS ${make_example_apps_exe})
    execute_process(COMMAND ${make_example_apps_exe})
endif()

file(GLOB sources *.cpp)
foreach(source ${sources})
    get_filename_component(exe_name ${source} NAME_WE)
    message("Adding example app \"${exe_name}\" from ${source}")
    hello_imgui_add_app(${exe_name} ${source})
    target_include_directories(${exe_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
endforeach()
