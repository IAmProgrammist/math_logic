add_subdirectory(libs)

add_executable(lab1 lab1/main.c)
target_link_libraries(lab1 alg)