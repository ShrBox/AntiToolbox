cmake_minimum_required(VERSION 3.21)
project(AntiToolbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/x64/)
set(TARGET_DIR ${CMAKE_SOURCE_DIR}/x64/Release/)

file(GLOB SRC_FILES
        ${PROJECT_SOURCE_DIR}/AntiToolbox/*.cpp
        ${PROJECT_SOURCE_DIR}/AntiToolbox/*.h
        )

add_definitions(-D"NDEBUG" -D"LLMONEY_EXPORTS" -D"_CRT_SECURE_NO_WARNINGS" -D"_WINDOWS"
        -D"_USRDLL" -D"_AMD64_" -D"NOMINMAX" -D"_WINDLL" -D"_UNICODE" -D"UNICODE")

add_compile_options(
        /permissive- /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /O2 /sdl /Zc:inline /fp:precise
        /errorReport:prompt /WX- /Zc:forScope /Gd /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:column 
)

add_link_options(
        /MANIFEST /LTCG:incremental /NXCOMPAT /DEBUG /DLL /MACHINE:X64 /OPT:REF /INCREMENTAL:NO /SUBSYSTEM:WINDOWS
        /MANIFESTUAC:NO /OPT:ICF /ERRORREPORT:PROMPT /NOLOGO /DELAYLOAD:"bedrock_server.dll" /TLBID:1
)

add_library(AntiToolbox SHARED ${SRC_FILES})
target_link_libraries(AntiToolbox "${PROJECT_SOURCE_DIR}/SDK/Lib/LiteLoader.lib"
        "${PROJECT_SOURCE_DIR}/SDK/Lib/bedrock_server_api.lib"
        "${PROJECT_SOURCE_DIR}/SDK/Lib/bedrock_server_var.lib"
        "${PROJECT_SOURCE_DIR}/SDK/Lib/SymDBHelper.lib")

include_directories(${PROJECT_SOURCE_DIR}/SDK/Header)
include_directories(${PROJECT_SOURCE_DIR}/SDK/Header/third-party)
include_directories(${PROJECT_SOURCE_DIR}/Header)

add_custom_command(TARGET AntiToolbox PRE_BUILD
        COMMAND cmd /c ${PROJECT_SOURCE_DIR}/prepareLib.cmd ${PROJECT_SOURCE_DIR}
        COMMENT "Preparing Library"
        )
