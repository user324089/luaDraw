add_library (constructions SHARED constructions.cpp)
target_include_directories (constructions PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries (constructions ${LUA_LIBRARIES})
target_compile_features(constructions PRIVATE cxx_std_17)
set_target_properties (constructions PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:main>
    PREFIX ""
    )

if (NOT WIN32)
    target_compile_options (constructions PRIVATE -fPIC)
endif ()

set (MSVCDebugCompileFlags /Od /Zi /W4)
set (MSVCReleaseCompileFlags /Ot)

set (nonMSVCDebugCompileFlags -O0 -g -Wall -Wextra)
set (nonMSVCReleaseCompileFlags -O2)

if (MSVC)
    target_compile_options (constructions PRIVATE $<$<CONFIG:Debug>:${MSVCDebugCompileFlags}>)
    target_compile_options (constructions PRIVATE $<$<CONFIG:Release>:${MSVCReleaseCompileFlags}>)
else ()
    target_compile_options (constructions PRIVATE $<$<CONFIG:Debug>:${nonMSVCDebugCompileFlags}>)
    target_compile_options (constructions PRIVATE $<$<CONFIG:Release>:${nonMSVCReleaseCompileFlags}>)
endif()

target_compile_definitions (constructions PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_definitions (constructions PRIVATE $<$<CONFIG:Release>:NDEBUG>)
