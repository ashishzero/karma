/*
 * Unity build file
*/

//
// Preprocessor and include macros
//

/*
 * Build Selector 
 *  default:           *BUILD_DEBUG* if *_DEBUG* or *DEBUG* is defined else *BUILD_RELEASE* unless overidden by debugger
 *  - BUILD_DEBUG:     Generates debug information to work with debugger (no optimization)
 *  - BUILD_DEVELOPER: Gennerates with optimization enabled including _some_ debug information and developer enabled options
 *  - BUILD_RELEASE:   Production build
*/
// uncomment to use macro
// #define BUILD_DEBUG

/*
 * Entry point
 * default:                 Uses windows entry point in *Windows*
 *  - SYSTEMS_RUN_WITH_CRT: Uses C Runtime entry point in *Windows*
*/
// uncomment to use macro
// #define SYSTEMS_RUN_WITH_CRT

/*
 * Systems
 * default:                     Builds systems implementation
 *    - EXCLUDE_SYSTEMS_BUILD   Systems is not build
*/
// uncomment to exclude systems from build process
// #define EXCLUDE_SYSTEMS_BUILD


#define BUILD_RENDERER_NONE 0
#define BUILD_RENDERER_ALL 1
#define BUILD_RENDERER_OPENGL 2

/*
 * Renderer
 * default:                         Builds all renderer
 *      - BUILD_RENDERER_NONE       Builds none renderer
 *      - BUILD_RENDERER_ALL        Builds all renderer
 *      - BUILD_RENDERER_OPENGL     Builds OpenGL renderer
*/
#define BUILD_RENDERER_TYPE BUILD_RENDERER_ALL

/*
 * Default Temporary Memory Size
 * default: 32 Mega Bytes
*/
// uncomment to use change default temporary memory size
// #define DEFAULT_TEMPORARY_MEMORY_SIZE 1024*1024*32

/*
 * Libraries That are compiled as seperate translation unit
 *          - glad (src/glad)
 *          - imgui (src/imgui)
 *          - pcg (src/pcg)
*/

//
// Platform Independent source files
//

#include "shared.h"

// Entry point
#include "main.cpp"

#include "shared_newdelete.cpp"

#include "maths.cpp"

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
#include "string.cpp"

#include "ucd.cpp"
#include "type_info.cpp"

#include "gfx_renderer.cpp"

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
#include "tokenizer.cpp"
#include "prebuild.cpp"
#endif

//
//
//

#ifndef EXCLUDE_SYSTEMS_BUILD

#if defined(TARGET_WINDOWS)
#include "windows/systems_windows.cpp"
#else
#error "Platform not supported!"
#endif

#endif

//
//
//

#ifndef BUILD_RENDERER_TYPE
#define BUILD_RENDERER_TYPE BUILD_RENDERER_ALL
#endif

#if BUILD_RENDERER_TYPE == BUILD_RENDERER_ALL
#define INCLUDE_OPENGL_RENDERER
#endif

#if BUILD_RENDERER_TYPE == BUILD_RENDERER_OPENGL
#define INCLUDE_OPENGL_RENDERER
#endif

#ifdef INCLUDE_OPENGL_RENDERER
#include "gfx_opengl.cpp"
#endif

