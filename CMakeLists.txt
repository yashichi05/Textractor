# CMakeList.txt: 最上層的 CMake 專案檔，執行全域設定
# 並在這裡包含子專案。
cmake_minimum_required(VERSION 3.16)
# 專案名稱
project(Textractor)
# 設定CMAKE_MODULE_PATH 值${CMAKE_MODULE_PATH} + ${CMAKE_CURRENT_SOURCE_DIR}/cmake(用來引用cmake script)
# 中間空白會忽略
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#設定MSVC編譯選項
#/std	C + + 標準版本相容性選取器
#/MP	同時建置多重原始程式檔。
#/wdnnnn	/wd4326會隱藏編譯器警告 C4326
#/D 設定常數
#/DUNICODE /D_UNICODE 使用unicode

add_compile_options(
  /std:c++17
  /MP
  /wd4018 # signed/unsigned mismatch
  /DVERSION="${VERSION}"
  /DUNICODE # config.pri
  /D_UNICODE
)
# 如果為8則為64位元
if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
  # 設定CMAKE_FINAL_OUTPUT_DIRECTORY
  set(CMAKE_FINAL_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/builds/${CMAKE_BUILD_TYPE}_x64)
  # 設定函式庫 (由texthook 函式庫連結)
  link_directories(x64libs)
else()
  set(CMAKE_FINAL_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/builds/${CMAKE_BUILD_TYPE}_x86)
  link_directories(x86libs)
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_FINAL_OUTPUT_DIRECTORY}>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_FINAL_OUTPUT_DIRECTORY}>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_FINAL_OUTPUT_DIRECTORY}>)
#頭文件所在地
include_directories(include)
# 新增程式庫(設定target_precompile_headers用，是不是text.cpp 無所謂?)
add_library(pch text.cpp)
#編譯pch時，預先編譯common.h
target_precompile_headers(pch PUBLIC include/common.h)

#GLOB產生名為ASSETS 的檔案清單(陣列)
#COPY將此清單複製到CMAKE_FINAL_OUTPUT_DIRECTORY
file(GLOB ASSETS assets/*)
file(COPY ${ASSETS} DESTINATION ${CMAKE_FINAL_OUTPUT_DIRECTORY})

add_library(text text.cpp)
#編譯時提供的變數 TEXT_LANGUAGE，設定text definitions(編譯語系)
target_compile_definitions(text PRIVATE ${TEXT_LANGUAGE})
#連結library
link_libraries(text)

# 包含需建置的子專案
add_subdirectory(GUI)
add_subdirectory(texthook)
add_subdirectory(extensions)
add_subdirectory(test)
if (DEFINED VERSION)
  add_subdirectory(GUI/host)
endif()
#add_subdirectory(GUI/host)
