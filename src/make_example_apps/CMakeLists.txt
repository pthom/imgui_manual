add_executable(make_example_apps make_example_apps.cpp)
target_link_libraries(make_example_apps PRIVATE source_parse)
target_compile_definitions(make_example_apps PRIVATE -DIMGUI_MANUAL_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
file(CREATE_LINK ${CMAKE_BINARY_DIR}/src/assets ${CMAKE_CURRENT_BINARY_DIR}/assets SYMBOLIC)
