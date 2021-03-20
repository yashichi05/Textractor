include_directories(. util)

if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
set(texthook_src
  main.cc
  texthook.cc
  hookfinder.cc
  engine/match.cc
  engine/match64.cc
  engine/native/pchooks.cc
  util/ithsys/ithsys.cc
  util/util.cc
)
else()
set(texthook_src
  main.cc
  texthook.cc
  hookfinder.cc
  engine/engine.cc
  engine/match.cc
  engine/match32.cc
  engine/native/pchooks.cc
  util/util.cc
  util/ithsys/ithsys.cc
  util/disasm/disasm.cc
  util/memdbg/memsearch.cc
)
endif()

# 新增名為texthook 的程式庫，MODULE 為設定動態連結程式庫的意思
add_library(texthook MODULE ${texthook_src})
target_precompile_headers(texthook REUSE_FROM pch)

#texthook 與minhook程式庫連結
target_link_libraries(texthook minhook)
