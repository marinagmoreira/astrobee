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

#ifndef LOCALIZATION_COMMON_UTILITIES_H_
#define LOCALIZATION_COMMON_UTILITIES_H_

#include <config_reader/config_reader.h>
#include <ff_msgs/EkfState.h>
#include <ff_msgs/VisualLandmarks.h>
#include <localization_common/combined_nav_state.h>
#include <localization_common/combined_nav_state_covariances.h>
#include <localization_common/time.h>

#include <gtsam/geometry/Pose3.h>

#include <Eigen/Core>

#include <geometry_msgs/Pose.h>
#include <std_msgs/Header.h>

#include <string>

namespace localization_common {
Eigen::Isometry3d LoadTransform(config_reader::ConfigReader& config, const std::string& transform_config_name);

gtsam::Pose3 GtPose(const Eigen::Isometry3d& eigen_pose);

Eigen::Isometry3d EigenPose(const CombinedNavState& combined_nav_state);

// Returns pose in body frame
Eigen::Isometry3d EigenPose(const ff_msgs::VisualLandmarks& vl_features, const Eigen::Isometry3d& sensor_T_body);

// Returns pose in sensor frame
Eigen::Isometry3d EigenPose(const ff_msgs::VisualLandmarks& vl_features);

void SetEnvironmentConfigs(const std::string& astrobee_configs_path, const std::string& world);

ros::Time RosTimeFromHeader(const std_msgs::Header& header);

Time TimeFromHeader(const std_msgs::Header& header);

void TimeToHeader(const Time timestamp, std_msgs::Header& header);

template <typename VectorType, typename MsgVectorType>
VectorType VectorFromMsg(const MsgVectorType& msg_vector) {
  return VectorType(msg_vector.x, msg_vector.y, msg_vector.z);
}

template <typename VectorType, typename MsgVectorType>
void VectorToMsg(const VectorType& vector, MsgVectorType& msg_vector) {
  msg_vector.x = vector.x();
  msg_vector.y = vector.y();
  msg_vector.z = vector.z();
}

template <typename RotationType, typename MsgRotationType>
RotationType RotationFromMsg(const MsgRotationType& msg_rotation) {
  return RotationType(msg_rotation.w, msg_rotation.x, msg_rotation.y, msg_rotation.z);
}

template <typename RotationType, typename MsgRotationType>
void RotationToMsg(const RotationType& rotation, MsgRotationType& msg_rotation) {
  msg_rotation.w = rotation.w();
  msg_rotation.x = rotation.x();
  msg_rotation.y = rotation.y();
  msg_rotation.z = rotation.z();
}

gtsam::Pose3 PoseFromMsg(const geometry_msgs::Pose& msg_pose);

void PoseToMsg(const gtsam::Pose3& pose, geometry_msgs::Pose& msg_pose);

void VariancesToCovDiag(const Eigen::Vector3d& variances, float* const cov_diag);

Eigen::Vector3d CovDiagToVariances(const float* const cov_diag);

CombinedNavState CombinedNavStateFromMsg(const ff_msgs::EkfState& loc_msg);

void CombinedNavStateToMsg(const CombinedNavState& combined_nav_state, ff_msgs::EkfState& loc_msg);

CombinedNavStateCovariances CombinedNavStateCovariancesFromMsg(const ff_msgs::EkfState& loc_msg);

void CombinedNavStateCovariancesToMsg(const CombinedNavStateCovariances& covariances, ff_msgs::EkfState& loc_msg);
}  // namespace localization_common

#endif  // LOCALIZATION_COMMON_UTILITIES_H_