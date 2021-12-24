# 依赖的 Qt 组件
set(QT_LIBS
    WebSockets Widgets
)

set(TS_FILES "qaria2_zh_CN.ts")

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
    "aria2optionsbuilder.h"
    "statusdelegate.h" "statusdelegate.cpp"
    "newdownloaddialog.h" "newdownloaddialog.cpp" "newdownloaddialog.ui"
    "newbittorrentdialog.h" "newbittorrentdialog.cpp" "newbittorrentdialog.ui"
    "progressitemdelegate.h" "progressitemdelegate.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/resource.rc"
)

add_qt_executable(TARGET qaria2
    QT_LIBS ${QT_LIBS}
    SOURCES ${SOURCES}
    TS_FILES ${TS_FILES}
)

target_compile_features(qaria2 PRIVATE cxx_user_literals)
target_compile_options(qaria2 PRIVATE ${PROJECT_WARNING_FLAGS})
target_precompile_headers(qaria2 PRIVATE "common.h")
