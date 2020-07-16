function(add_one_cpp_test test_cpp_file)
    get_filename_component(test_exe_file ${test_cpp_file} NAME_WE)

    add_executable(${test_exe_file} ${test_cpp_file})
    target_link_libraries(${test_exe_file} PRIVATE source_parse)
    target_include_directories(${test_exe_file} PRIVATE ${doctest_dir})
    add_test(NAME ${test_exe_file} COMMAND ${test_exe_file})
endfunction()

file(CREATE_LINK ${CMAKE_BINARY_DIR}/src/assets ${CMAKE_CURRENT_BINARY_DIR}/assets SYMBOLIC)

add_one_cpp_test(ImGuiHeaderDocParser_test.cpp)
add_one_cpp_test(HeaderTree_test.cpp)
