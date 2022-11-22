cmake_minimum_required (VERSION 3.12)

set(PlayerSource main.cpp)

# WIN32(gui program;need WinMain entry point)
add_executable (player ${PlayerSource})

target_link_libraries(player Common)
target_link_libraries(player ${Boost_LIBRARIES})