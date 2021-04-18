#導入QtUtils.cmake 檔案
include(QtUtils)
#調用巨集
msvc_registry_search()
find_qt5(Core Widgets TextToSpeech WebSockets)
#使用 37 規則
#https://cmake.org/cmake/help/latest/policy/CMP0037.html
cmake_policy(SET CMP0037 OLD)

link_directories(mecab)
#新增MODULE程式庫(dll)
add_library(Bing\ Translate MODULE bingtranslate.cpp translatewrapper.cpp network.cpp extensionimpl.cpp)
add_library(Copy\ to\ Clipboard MODULE copyclipboard.cpp extensionimpl.cpp)
add_library(DeepL\ Translate MODULE deepltranslate.cpp translatewrapper.cpp network.cpp extensionimpl.cpp)
add_library(DevTools\ DeepL\ Translate MODULE devtoolsdeepltranslate.cpp devtools.cpp translatewrapper.cpp network.cpp extensionimpl.cpp)
add_library(Extra\ Newlines MODULE extranewlines.cpp extensionimpl.cpp)
add_library(Extra\ Window MODULE extrawindow.cpp extensionimpl.cpp)
add_library(Google\ Translate MODULE googletranslate.cpp translatewrapper.cpp network.cpp extensionimpl.cpp)
add_library(Lua MODULE lua.cpp extensionimpl.cpp)
add_library(Regex\ Filter MODULE regexfilter.cpp extensionimpl.cpp)
add_library(Remove\ Repeated\ Characters MODULE removerepeatchar.cpp extensionimpl.cpp)
add_library(Remove\ Repeated\ Phrases MODULE removerepeatphrase.cpp extensionimpl.cpp)
add_library(Remove\ Repeated\ Phrases\ 2 MODULE removerepeatphrase2.cpp extensionimpl.cpp)
add_library(Remove\ 30\ Repeated\ Sentences MODULE removerepeatsentence.cpp extensionimpl.cpp)
add_library(Replacer MODULE replacer.cpp extensionimpl.cpp)
add_library(Styler MODULE styler.cpp extensionimpl.cpp)
add_library(Thread\ Linker MODULE threadlinker.cpp extensionimpl.cpp)

#重用在pch編譯的common.h
target_precompile_headers(Bing\ Translate REUSE_FROM pch)
target_precompile_headers(Copy\ to\ Clipboard REUSE_FROM pch)
target_precompile_headers(DeepL\ Translate REUSE_FROM pch)
target_precompile_headers(DevTools\ DeepL\ Translate REUSE_FROM pch)
target_precompile_headers(Extra\ Newlines REUSE_FROM pch)
target_precompile_headers(Extra\ Window REUSE_FROM pch)
target_precompile_headers(Google\ Translate REUSE_FROM pch)
target_precompile_headers(Lua REUSE_FROM pch)
target_precompile_headers(Regex\ Filter REUSE_FROM pch)
target_precompile_headers(Remove\ Repeated\ Characters REUSE_FROM pch)
target_precompile_headers(Remove\ Repeated\ Phrases REUSE_FROM pch)
target_precompile_headers(Remove\ Repeated\ Phrases\ 2 REUSE_FROM pch)
target_precompile_headers(Remove\ 30\ Repeated\ Sentences REUSE_FROM pch)
target_precompile_headers(Replacer REUSE_FROM pch)
target_precompile_headers(Styler REUSE_FROM pch)
target_precompile_headers(Thread\ Linker REUSE_FROM pch)
#程式庫連結程式庫 winhttp、Qt5::Widgets
target_link_libraries(Bing\ Translate winhttp Qt5::Widgets)
target_link_libraries(DeepL\ Translate winhttp Qt5::Widgets)
target_link_libraries(DevTools\ DeepL\ Translate shell32 winhttp Qt5::Widgets Qt5::WebSockets)
target_link_libraries(Extra\ Window Qt5::Widgets libmecab Qt5::TextToSpeech)
target_link_libraries(Google\ Translate winhttp Qt5::Widgets)
target_link_libraries(Lua lua53 Qt5::Widgets)
target_link_libraries(Regex\ Filter Qt5::Widgets)
target_link_libraries(Styler Qt5::Widgets)
target_link_libraries(Thread\ Linker Qt5::Widgets)

#執行命令
add_custom_target(Cleaner ALL COMMAND del *.xdll WORKING_DIRECTORY ${CMAKE_FINAL_OUTPUT_DIRECTORY})

# Qt5WebSocket.dll不存在於CMAKE_FINAL_OUTPUT_DIRECTORY(build資料夾)
# 產生需要的 qt dll 檔
# POST_BUILD 在所有規則運行後執行
# CMAKE_COMMAND = cmd 的cmake 開頭指令
# 設定PATH = PATH值 + qt/bin資料夾位置; 
# qt5_install_prefix 由qtutils.cmake 設定的值
# Qt5::windeployqt --dir 等於  windeployqt.exe --dir
if (NOT EXISTS ${CMAKE_FINAL_OUTPUT_DIRECTORY}/Qt5WebSockets.dll AND NOT EXISTS ${CMAKE_FINAL_OUTPUT_DIRECTORY}/Qt5WebSocketsd.dll)
    add_custom_command(TARGET DevTools\ DeepL\ Translate
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"
        COMMAND set PATH=%PATH%$<SEMICOLON>${qt5_install_prefix}/bin
        COMMAND Qt5::windeployqt --dir ${CMAKE_FINAL_OUTPUT_DIRECTORY} "${CMAKE_FINAL_OUTPUT_DIRECTORY}/DevTools\ DeepL\ Translate.dll"
    )
endif() 

file(GLOB MECAB mecab/*)
file(COPY ${MECAB} DESTINATION ${CMAKE_FINAL_OUTPUT_DIRECTORY})