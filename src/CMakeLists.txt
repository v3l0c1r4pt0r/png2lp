find_package(PNG REQUIRED)

add_library(oki STATIC oki.c)
add_library(tty STATIC tty.c)
add_executable(png2lp png2lp.c binrd.c simplepng.c list.c printer.c oki tty)
target_include_directories(png2lp PRIVATE ${PNG_INCLUDE_DIR})
target_link_libraries(png2lp ${PNG_LIBRARY})
message("l:${PNG_LIBRARY}, i:${PNG_INCLUDE_DIR}")
