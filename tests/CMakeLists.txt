
# Add a test executable
add_executable(morsel_test morsel_test.cpp)

# Link the morsel_test executable with your library and any necessary libraries
target_link_libraries(morsel_test /usr/local/lib/libsqlparser.so /usr/local/lib/libpg_query.so)
target_link_libraries(morsel_test srclib )

# Add the morsel_test to CTest
add_test(NAME MorselTest COMMAND morsel_test)
