// Copyright 2024 KAS Lab
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

#include "ament_index_cpp/get_package_share_directory.hpp"

#include "gtest/gtest.h"
#include "plansys2_downward_planner/downward_planner.hpp"

#include "pluginlib/class_loader.hpp"
#include "pluginlib/class_list_macros.hpp"
#include "plansys2_core/PlanSolverBase.hpp"

void test_plan_generation(const std::string & argument = "--alias lama-first")
{
  std::string pkgpath = ament_index_cpp::get_package_share_directory("plansys2_downward_planner");
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple.pddl");
  std::string domain_str((
      std::istreambuf_iterator<char>(domain_ifs)),
    std::istreambuf_iterator<char>());

  std::ifstream problem_ifs(pkgpath + "/pddl/problem_simple_1.pddl");
  std::string problem_str((
      std::istreambuf_iterator<char>(problem_ifs)),
    std::istreambuf_iterator<char>());

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::DownwardPlanner>();
  planner->configure(node, "Downward");
  node->set_parameter(rclcpp::Parameter("Downward.arguments", argument));

  auto plan = planner->getPlan(domain_str, problem_str, "generate_plan_good");

  ASSERT_TRUE(plan);
  ASSERT_EQ(plan.value().items.size(), 3);
  ASSERT_EQ(plan.value().items[0].action, "(move leia kitchen bedroom)");
  ASSERT_EQ(plan.value().items[1].action, "(approach leia bedroom jack)");
  ASSERT_EQ(plan.value().items[2].action, "(talk leia jack jack m1)");
}

std::optional<std::filesystem::path> test_folder_creation(
  const std::string & output_dir = "", const std::string & node_namespace = "")
{
  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::DownwardPlanner>();

  planner->configure(node, "Downward");
  if (!output_dir.empty()) {
    node->set_parameter(rclcpp::Parameter("Downward.output_dir", output_dir));
  }

  return planner->create_folders(node_namespace);
}

TEST(downward_planner, generate_plan_good)
{
  test_plan_generation();
}

// TEST(downward_planner, generate_plan_good_with_argument)
// {
//   test_plan_generation("-e");
// }

TEST(downward_planner, load_popf_plugin)
{
  try {
    pluginlib::ClassLoader<plansys2::PlanSolverBase> lp_loader(
      "plansys2_core", "plansys2::PlanSolverBase");
    plansys2::PlanSolverBase::Ptr plugin =
      lp_loader.createUniqueInstance("plansys2/DownwardPlanner");
    ASSERT_TRUE(true);
  } catch (std::exception & e) {
    std::cerr << e.what() << std::endl;
    ASSERT_TRUE(false);
  }
}

TEST(downward_planner, check_1_ok_domain)
{
  std::string pkgpath = ament_index_cpp::get_package_share_directory("plansys2_downward_planner");
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_1_ok.pddl");
  std::string domain_str((
      std::istreambuf_iterator<char>(domain_ifs)),
    std::istreambuf_iterator<char>());

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::DownwardPlanner>();
  planner->configure(node, "Downward");

  bool result = planner->isDomainValid(domain_str, "check_1_ok_domain");

  ASSERT_TRUE(result);
}

TEST(downward_planner, check_2_error_domain)
{
  std::string pkgpath = ament_index_cpp::get_package_share_directory("plansys2_downward_planner");
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_2_error.pddl");
  std::string domain_str((
      std::istreambuf_iterator<char>(domain_ifs)),
    std::istreambuf_iterator<char>());

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::DownwardPlanner>();
  planner->configure(node, "Downward");

  bool result = planner->isDomainValid(domain_str, "check_2_error_domain");

  ASSERT_FALSE(result);
}

TEST(downward_planner, generate_plan_unsolvable)
{
  std::string pkgpath = ament_index_cpp::get_package_share_directory("plansys2_downward_planner");
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple.pddl");
  std::string domain_str((
      std::istreambuf_iterator<char>(domain_ifs)),
    std::istreambuf_iterator<char>());

  std::ifstream problem_ifs(pkgpath + "/pddl/problem_simple_2.pddl");
  std::string problem_str((
      std::istreambuf_iterator<char>(problem_ifs)),
    std::istreambuf_iterator<char>());

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::DownwardPlanner>();
  planner->configure(node, "Downward");

  auto plan = planner->getPlan(domain_str, problem_str);

  ASSERT_FALSE(plan);
}

TEST(downward_planner, generate_plan_error)
{
  std::string pkgpath = ament_index_cpp::get_package_share_directory("plansys2_downward_planner");
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple.pddl");
  std::string domain_str((
      std::istreambuf_iterator<char>(domain_ifs)),
    std::istreambuf_iterator<char>());

  std::ifstream problem_ifs(pkgpath + "/pddl/problem_simple_3.pddl");
  std::string problem_str((
      std::istreambuf_iterator<char>(problem_ifs)),
    std::istreambuf_iterator<char>());

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::DownwardPlanner>();
  planner->configure(node, "Downward");

  auto plan = planner->getPlan(domain_str, problem_str);

  ASSERT_FALSE(plan);
}

TEST(downward_planner, create_folder_default)
{
  const auto output_dir = test_folder_creation();
  ASSERT_TRUE(output_dir.has_value());
  EXPECT_EQ(output_dir.value(), std::filesystem::temp_directory_path());
}

TEST(downward_planner, create_folder_custom_path)
{
  const auto test_path = std::filesystem::temp_directory_path() / "test" / "path" / "one";
  const auto output_dir = test_folder_creation(test_path);
  ASSERT_TRUE(output_dir.has_value());
  EXPECT_EQ(output_dir.value(), test_path);
}

TEST(downward_planner, create_folder_custom_path_and_namespace)
{
  const auto test_path = std::filesystem::temp_directory_path() / "test" / "path" / "two";
  const auto test_namespace = "/test/node";
  const auto output_dir = test_folder_creation(test_path, test_namespace);
  ASSERT_TRUE(output_dir.has_value());
  EXPECT_EQ(output_dir.value(), test_path / "test" / "node");
}

TEST(downward_planner, create_folder_filesystem_error)
{
  const auto test_path = std::filesystem::temp_directory_path() / "test" / "path" / "three";

  // Create a file at the test path to force a filesystem error
  std::ofstream out(test_path);
  out << "random text\n";
  out.close();

  const auto test_namespace = "/test/node";
  const auto output_dir = test_folder_creation(test_path, test_namespace);
  ASSERT_FALSE(output_dir.has_value());
}

int main(int argc, char ** argv)
{
  testing::InitGoogleTest(&argc, argv);
  rclcpp::init(argc, argv);

  return RUN_ALL_TESTS();
}
