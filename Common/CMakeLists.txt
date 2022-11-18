cmake_minimum_required (VERSION 3.12)

set(CMAKE_CXX_STANDARD 11)

set(CommonSourceFile Log.h Log.cpp)
list(APPEND CommonSourceFile LogWriter.h LogWriter.cpp)

if(MSVC)
add_definitions( -bigobj)
endif()

add_library (Common ${CommonSourceFile})
if(WIN32)
target_link_libraries(Common Crypt32.lib)
endif()
