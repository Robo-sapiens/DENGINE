add_compile_options(--coverage)
add_executable(test_all test_all.cpp)
target_link_libraries(test_all
        gtest
        gcov
        graphics)
