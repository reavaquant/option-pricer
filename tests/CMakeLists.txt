add_executable(test_options test_options.cpp)
target_link_libraries(test_options PRIVATE option_pricer_lib)
add_test(NAME options COMMAND test_options)

add_executable(test_pricers test_pricers.cpp)
target_link_libraries(test_pricers PRIVATE option_pricer_lib)
add_test(NAME pricers COMMAND test_pricers)

add_executable(test_binarytree test_binarytree.cpp)
target_link_libraries(test_binarytree PRIVATE option_pricer_lib)
add_test(NAME binarytree COMMAND test_binarytree)