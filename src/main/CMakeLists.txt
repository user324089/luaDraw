add_executable (main main.cpp shapeDrawer.cpp shapeDrawer.hpp shapeStorage.cpp shapeStorage.hpp configInterpreter.cpp configInterpreter.hpp inputHandlers.cpp inputHandlers.hpp colorStorage.cpp colorStorage.hpp configureConstants.cpp luaDraw.cpp luaDraw.hpp)
target_include_directories (main PRIVATE ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${MY_OPENGL_WRAPPER_INCLUDE_DIR} ${LUA_INCLUDE_DIR})
target_link_libraries (main glfw glew_s ${OPENGL_LIBRARIES} ${MY_OPENGL_WRAPPER_LIBRARIES} ${LUA_LIBRARIES})
target_compile_definitions (main PRIVATE GLEW_NO_GLU)
target_compile_features(main PRIVATE cxx_std_17)
set_target_properties (main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (MSVCDebugCompileFlags /Od /Zi /W4)
set (MSVCReleaseCompileFlags /Ot)

set (nonMSVCDebugCompileFlags -O0 -g -Wall -Wextra)
set (nonMSVCReleaseCompileFlags -O2)

if (MSVC)
    target_compile_options (main PRIVATE $<$<CONFIG:Debug>:${MSVCDebugCompileFlags}>)
    target_compile_options (main PRIVATE $<$<CONFIG:Release>:${MSVCReleaseCompileFlags}>)
else ()
    target_compile_options (main PRIVATE $<$<CONFIG:Debug>:${nonMSVCDebugCompileFlags}>)
    target_compile_options (main PRIVATE $<$<CONFIG:Release>:${nonMSVCReleaseCompileFlags}>)
endif()

target_compile_definitions (main PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_definitions (main PRIVATE $<$<CONFIG:Release>:NDEBUG>)
