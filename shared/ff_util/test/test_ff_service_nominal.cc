/* Copyright (c) 2017, United States Government, as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * 
 * All rights reserved.
 * 
 * The Astrobee platform is licensed under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with the
 * License. You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

// Test service
#include <ff_util/ff_component.h>
#include <ff_util/ff_service.h>
#include <ff_common/ff_ros.h>
#include <gtest/gtest.h>

#include <ff_msgs/srv/set_rate.hpp>
#include <string>

FF_DEFINE_LOGGER("test_ff_service_nominal")

bool test_done = false;

class Client : public rclcpp::Node {
 public :
  Client() : Node("client_node") {
    client_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    timer_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

    client_ptr_ = this->create_client<ff_msgs::srv::SetRate>("/testing/set_rate", rmw_qos_profile_services_default,
                                                            client_cb_group_);
    timer_ptr_ = this->create_wall_timer((std::chrono::duration<double>) 1, std::bind(&Client::TestCall, this),
                                            timer_cb_group_);
  }

  void TestCall() {
    // timer_ptr_->cancel();

    client_ptr_->wait_for_service((std::chrono::duration<double>) 10);

    auto response = std::make_shared<ff_msgs::srv::SetRate::Response>();

    FF_ERROR("before call service");

    // Test is valid for fun
    auto request = std::make_shared<ff_msgs::srv::SetRate::Request>();
    request->which = ff_msgs::srv::SetRate::Request::DISK_STATE;
    request->rate = 5.0;



    auto result_future = client_ptr_->async_send_request(request);
    std::future_status status =
      result_future.wait_for((std::chrono::duration<double>)10);  // timeout to guarantee a graceful finish
    if (status == std::future_status::ready) {
        response = result_future.get();
        FF_ERROR_STREAM("Received response");
    }

    FF_ERROR_STREAM("after wait--- ready:" << (status == std::future_status::ready)
                                           << " deferred:" << (status == std::future_status::deferred)
                                           << " timeout:" << (status == std::future_status::timeout));

    EXPECT_TRUE((status == std::future_status::ready));
    EXPECT_TRUE(response->success);
    EXPECT_EQ(response->status, "Which is disk state.");
    test_done = true;
  }

 private:
  rclcpp::CallbackGroup::SharedPtr client_cb_group_;
  rclcpp::CallbackGroup::SharedPtr timer_cb_group_;

  rclcpp::Client<ff_msgs::srv::SetRate>::SharedPtr client_ptr_;
  rclcpp::TimerBase::SharedPtr timer_ptr_;
};

TEST(ff_service, Nominal) {
  test_done = false;

  auto client_node = std::make_shared<Client>();
  FF_ERROR("Created Node");

  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(client_node);
  FF_ERROR("Added to executor");


  while (!test_done) {
    executor.spin_some();
  }
}

// Run all the tests that were declared with TEST()
int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  // google::InitGoogleLogging(argv[0]);
  rclcpp::init(argc, argv);
  int result = RUN_ALL_TESTS();
  rclcpp::shutdown();
  return result;
}
