FILE(GLOB sources
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    )
add_library(source_parse STATIC ${sources})
target_include_directories(source_parse PUBLIC
    ${fplus_dir}
    ${CMAKE_CURRENT_LIST_DIR}/..
    )
target_link_libraries(source_parse PRIVATE hello_imgui)
