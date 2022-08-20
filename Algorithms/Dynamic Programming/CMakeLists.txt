add_executable(coin_change coin_change.cpp)
target_compile_options(coin_change PRIVATE -Wno-deprecated-declarations)

add_executable(equal equal.cpp)
target_compile_options(coin_change PRIVATE -Wno-deprecated-declarations)

add_executable(knapsack knapsack.cpp)
target_link_libraries(knapsack PRIVATE doctest)
