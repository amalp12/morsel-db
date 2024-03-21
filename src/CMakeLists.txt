add_subdirectory(./DB)

set(SOURCES morsel.cpp operators.cpp relcat.cpp parser.cpp attribute.cpp dispatcher.cpp init.cpp  lib.cpp loop_functions.cpp operators.cpp tuple.cpp test.cpp static.cpp)

add_library( srclib ${SOURCES} )
# add_executable(dbapp app.cpp)
add_executable(dbapp app.cpp)

target_link_libraries(dbapp /usr/local/lib/libsqlparser.so /usr/local/lib/libpg_query.so)
target_link_libraries(dbapp srclib )
