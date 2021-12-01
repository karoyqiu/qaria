# 默认警告
include(CheckWarningFlags)
check_warning_flags(CXX PROJECT_WARNING_FLAGS)

# 依赖的 Qt 组件
set(QT_LIBS
    WebSockets Widgets
)

set(TS_FILES "qaria_zh_CN.ts")

# 版本文件
configure_file("resource.rc.in" "resource.rc" @ONLY)

set(SOURCES
    "common.h"
    "main.cpp"
    "mainwindow.h" "mainwindow.cpp" "mainwindow.ui"
    "optionsdialog.h" "optionsdialog.cpp" "optionsdialog.ui"
    "aria2c.h" "aria2c.cpp"
    "downloadtablemodel.h" "downloadtablemodel.cpp"
    "datasizedelegate.h" "datasizedelegate.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/resource.rc"
)

add_qt_executable(TARGET qaria
    QT_LIBS ${QT_LIBS}
    SOURCES ${SOURCES}
    TS_FILES ${TS_FILES}
)

target_compile_options(qaria PRIVATE ${PROJECT_WARNING_FLAGS})
target_precompile_headers(qaria PRIVATE "common.h")
