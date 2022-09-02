add_executable(pacific_atlantic pacific_atlantic.cpp)
# MSVC stack size increase
target_link_options(pacific_atlantic PRIVATE LINKER:/stack:12582912)
target_link_libraries(pacific_atlantic PRIVATE doctest)
