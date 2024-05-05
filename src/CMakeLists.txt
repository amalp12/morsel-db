set(SOURCES morsel.cpp operators.cpp relcat.cpp parser.cpp attribute.cpp dispatcher.cpp init.cpp  lib.cpp loop_functions.cpp operators.cpp tuple.cpp test.cpp static.cpp b_plus_tree.cpp)

add_library( srclib ${SOURCES} )
# add_executable(dbapp app.cpp)
add_executable(dbapp app.cpp)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_link_libraries(dbapp /usr/local/lib/libsqlparser_debug.so )
    # target_link_libraries(your_target_name debug debug_library_name)
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    target_link_libraries(dbapp /usr/local/lib/libsqlparser_release.so )
    # target_link_libraries(your_target_name optimized release_library_name)
endif()

target_link_libraries(dbapp numa)

target_link_libraries(dbapp srclib )
