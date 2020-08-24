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

#include <ff_util/ff_names.h>
#include <imu_augmentor/imu_augmentor_nodelet.h>

#include <glog/logging.h>

namespace imu_augmentor {

ImuAugmentorNodelet::ImuAugmentorNodelet() {}

void ImuAugmentorNodelet::Initialize(ros::NodeHandle* nh) {
  SubscribeAndAdvertise(nh);
  Run();
}

void ImuAugmentorNodelet::SubscribeAndAdvertise(ros::NodeHandle* nh) {
  state_pub_ = nh->advertise<ff_msgs::EkfState>(TOPIC_GNC_EKF, 1);
  imu_sub_ =
      nh->subscribe(TOPIC_HARDWARE_IMU, 1, &ImuAugmentorNodelet::ImuCallback, this, ros::TransportHints().tcpNoDelay());
  state_sub_ = nh->subscribe(TOPIC_GRAPH_LOC_STATE, 1, &ImuAugmentorNodelet::LocalizationStateCallback, this,
                             ros::TransportHints().tcpNoDelay());
}

void ImuAugmentorNodelet::ImuCallback(const sensor_msgs::Imu::ConstPtr& imu_msg) {
  imu_augmentor_wrapper_.ImuCallback(*imu_msg);
  PublishLatestImuAugmentedLocalizationState();
}

void ImuAugmentorNodelet::LocalizationStateCallback(const ff_msgs::EkfState::ConstPtr& loc_msg) {
  imu_augmentor_wrapper_.LocalizationStateCallback(*loc_msg);
}

void ImuAugmentorNodelet::PublishLatestImuAugmentedLocalizationState() {
  ff_msgs::EkfState latest_imu_augmented_loc_msg;
  if (!imu_augmentor_wrapper_.LatestImuAugmentedLocalizationMsg(latest_imu_augmented_loc_msg)) {
    LOG(ERROR) << "PublishLatestImuAugmentedLocalizationState: Failed to get latest imu augmented loc msg.";
    return;
  }
  state_pub_.publish(latest_imu_augmented_loc_msg);
}

void ImuAugmentorNodelet::Run() {
  ros::Rate rate(100);  // 100 Hz
  while (ros::ok()) {
    ros::spinOnce();
    rate.sleep();
  }
}
}  // namespace imu_augmentor

PLUGINLIB_EXPORT_CLASS(imu_augmentor::ImuAugmentorNodelet, nodelet::Nodelet);