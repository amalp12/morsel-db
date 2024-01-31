set (SOURCES morsel.cpp operators.cpp relcat.cpp)

#add_library( srclib ${SOURCES} )
add_executable(dbapp app.cpp)

target_link_libraries(dbapp /usr/local/lib/libsqlparser.so /usr/local/lib/libpg_query.so)
#target_link_libraries(dbapp srclib )
