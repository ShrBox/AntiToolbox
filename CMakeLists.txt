cmake_minimum_required(VERSION 3.21)
project(AntiToolbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

file(GLOB SRC_FILES
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.h
        )

add_definitions(-D"NDEBUG" -D"LLMONEY_EXPORTS" -D"_CRT_SECURE_NO_WARNINGS" -D"_WINDOWS"
        -D"_USRDLL" -D"_AMD64_" -D"NOMINMAX" -D"_WINDLL" -D"_UNICODE" -D"UNICODE")

add_compile_options(
        /permissive- /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /O2 /sdl /Zc:inline /fp:precise
        /errorReport:prompt /WX- /Zc:forScope /Gd /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:column /utf-8
)

add_link_options(
        /MANIFEST /LTCG:incremental /NXCOMPAT /DEBUG /DLL /MACHINE:X64 /OPT:REF /INCREMENTAL:NO /SUBSYSTEM:WINDOWS
        /MANIFESTUAC:NO /OPT:ICF /ERRORREPORT:PROMPT /NOLOGO /DELAYLOAD:"bedrock_server.dll" /TLBID:1
)

add_library(AntiToolbox SHARED ${SRC_FILES})
target_link_libraries(AntiToolbox "${PROJECT_SOURCE_DIR}/SDK/lib/LiteLoader.lib"
        "${PROJECT_SOURCE_DIR}/SDK/lib/bedrock_server_api.lib"
        "${PROJECT_SOURCE_DIR}/SDK/lib/bedrock_server_var.lib"
        "${PROJECT_SOURCE_DIR}/SDK/lib/SymDBHelper.lib")

include_directories(${PROJECT_SOURCE_DIR}/SDK/include)
include_directories(${PROJECT_SOURCE_DIR}/SDK/include/llapi)
include_directories(${PROJECT_SOURCE_DIR}/Header)

add_custom_command(TARGET AntiToolbox PRE_BUILD
        COMMAND cmd /c ${PROJECT_SOURCE_DIR}/prepare_libraries.cmd ${PROJECT_SOURCE_DIR}
        COMMENT "Preparing Library"
        )
