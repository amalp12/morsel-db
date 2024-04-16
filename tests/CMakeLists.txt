
# Add a test executable
add_executable(morsel_test morsel_test.cpp)

# Link the morsel_test executable with your library and any necessary libraries
# target_link_libraries(morsel_test /usr/local/lib/libsqlparser.so)
# target_link_libraries(morsel_test /usr/local/lib/libsqlparser_debug.so)  
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_link_libraries(morsel_test /usr/local/lib/libsqlparser_debug.so )
    # target_link_libraries(your_target_name debug debug_library_name)
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    target_link_libraries(morsel_test /usr/local/lib/libsqlparser_release.so )
    # target_link_libraries(your_target_name optimized release_library_name)
endif()
target_link_libraries(morsel_test srclib )

# Add the morsel_test to CTest
add_test(NAME MorselTest COMMAND morsel_test)
