ament_add_gtest(downward_test downward_test.cpp)
target_link_libraries(downward_test ${PROJECT_NAME} dl)
target_compile_definitions(downward_test PUBLIC "PLUGINLIB__DISABLE_BOOST_FUNCTIONS")
