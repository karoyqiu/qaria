macro(CHECK_WARNING_FLAGS LANG WARNING_FLAGS)
    if(CMAKE_${LANG}_COMPILER_ID STREQUAL "GNU" OR CMAKE_${LANG}_COMPILER_ID STREQUAL "Clang")
        set(CANDIDATE_FLAGS
            "-Wall"
            "-Wextra"
            "-Wpedantic"
            "-pedantic"
            "-Wformat=2"
            "-Werror=return-type"
            "-Werror=vexing-parse"
            "-Wno-unknown-pragmas"
            "-Wno-unused-local-typedefs"
        )
    elseif(CMAKE_${LANG}_COMPILER_ID STREQUAL "MSVC")
        set(CANDIDATE_FLAGS "/W4"
            "/experimental:external"
            "/external:anglebrackets"
            "/external:W0"
            "/utf-8"
        )
        set("CMAKE_INCLUDE_SYSTEM_FLAG_${LANG}" "/external:I")
    endif()

    include(CheckCompilerFlags)
    CHECK_COMPILER_FLAGS(${LANG} ${WARNING_FLAGS} ${CANDIDATE_FLAGS})
endmacro()
