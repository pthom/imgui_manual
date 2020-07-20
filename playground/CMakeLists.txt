file(GLOB sources *.cpp)
foreach(source ${sources})
    get_filename_component(exe_name ${source} NAME_WE)
    message("Adding exmple \"${exe_name}\" from ${source}")
    hello_imgui_add_app(${exe_name} ${source})
    target_include_directories(${exe_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
endforeach()

add_subdirectory(ExampleApps)
