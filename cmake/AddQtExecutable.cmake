# 写入 qrc 文件
function(write_qrc qrc files)
    set(xml "<RCC><qresource prefix=\"/i18n\">")

    foreach(f ${files})
        get_filename_component(fn "${f}" NAME)
        set(xml "${xml}<file>${fn}</file>")
    endforeach()

    set(xml "${xml}</qresource></RCC>")
    file(GENERATE OUTPUT "${qrc}" CONTENT "${xml}")
endfunction()

# 添加 Qt 应用
function(add_qt_executable)
    set(options CONSOLE)
    set(oneValueArgs TARGET)
    set(multiValueArgs QT_LIBS SOURCES TS_FILES)
    cmake_parse_arguments(AQE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(PROJECT_SOURCES ${AQE_SOURCES} ${AQE_TS_FILES})
    set(WIN32_APP "WIN32")
    
    if(AQE_CONSOLE)
        unset(WIN32_APP)
    endif()

    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_executable(${AQE_TARGET} ${WIN32_APP}
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
        )

        # Define target properties for Android with Qt 6 as:
        #    set_property(TARGET ${AQE_TARGET} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
        #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
        # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

        qt_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${AQE_TS_FILES})
    else()
        if(ANDROID)
            add_library(${AQE_TARGET} SHARED
                ${PROJECT_SOURCES}
            )

            # Define properties for Android with Qt 5 after find_package() calls as:
            #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
        else()
            add_executable(${AQE_TARGET} ${WIN32_APP}
                ${PROJECT_SOURCES}
            )
        endif()

        qt5_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${AQE_TS_FILES})
    endif()

    set(qrc "${CMAKE_CURRENT_BINARY_DIR}/i18n.qrc")
    write_qrc("${qrc}" "${QM_FILES}")
    add_custom_command(OUTPUT "${qrc}"
        COMMAND ${CMAKE_COMMAND} -E touch "${qrc}"
        DEPENDS ${QM_FILES} VERBATIM
    )
    target_sources(${AQE_TARGET} PRIVATE "${QM_FILES}" "${qrc}")

    foreach(L ${AQE_QT_LIBS})
        target_precompile_headers(${AQE_TARGET} PRIVATE <Qt${L}>)
        target_link_libraries(${AQE_TARGET} PRIVATE Qt${QT_VERSION_MAJOR}::${L})
    endforeach()

    set_target_properties(${AQE_TARGET} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER ${AQE_TARGET}.acompany.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )

    if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(${AQE_TARGET})
    endif()
endfunction()
