add_mlir_dialect_library(DBDialect
    DBDialect.cpp
    DBOps.cpp
    ADDITIONAL_HEADER_DIRS
    ${PROJECT_SOURCE_DIR}/include/DB

    DEPENDS
    MLIRDBOpsIncGen

    LINK_LIBS PUBLIC
    MLIRIR
)