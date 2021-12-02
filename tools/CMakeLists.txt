# 依赖的 Qt 组件
set(QT_LIBS
    Core
)

set(SOURCES
    "../src/common.h"
    "main.cpp"
)

add_qt_executable(TARGET "doc-parser"
    QT_LIBS ${QT_LIBS}
    SOURCES ${SOURCES}
)

target_compile_options("doc-parser" PRIVATE ${PROJECT_WARNING_FLAGS})
target_precompile_headers("doc-parser" PRIVATE "../src/common.h")
