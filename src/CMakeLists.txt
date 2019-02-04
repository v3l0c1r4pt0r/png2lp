find_package(PNG REQUIRED)

add_executable(png2lp png2lp.c binrd.c simplepng.c)
target_include_directories(png2lp PRIVATE ${PNG_INCLUDE_DIR})
target_link_libraries(png2lp ${PNG_LIBRARY})
message("l:${PNG_LIBRARY}, i:${PNG_INCLUDE_DIR}")
