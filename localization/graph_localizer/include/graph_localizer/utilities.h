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

#ifndef GRAPH_LOCALIZER_UTILITIES_H_
#define GRAPH_LOCALIZER_UTILITIES_H_

#include <config_reader/config_reader.h>
#include <ff_msgs/EkfState.h>
#include <ff_msgs/VisualLandmarks.h>
#include <graph_localizer/graph_loc_initialization.h>
#include <graph_localizer/graph_localizer.h>
#include <localization_measurements/combined_nav_state_covariances.h>
#include <localization_measurements/feature_point.h>
#include <localization_measurements/imu_measurement.h>

#include <gtsam/geometry/Pose3.h>

#include <Eigen/Core>

#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <std_msgs/Header.h>

#include <deque>
#include <string>
#include <vector>

namespace graph_localizer {
gtsam::Pose3 GtPose(const Eigen::Isometry3d &eigen_pose);

Eigen::Isometry3d LoadTransform(config_reader::ConfigReader &config, const std::string &transform_config_name);

void SetEnvironmentConfigs(const std::string &astrobee_configs_path, const std::string &world);

void EstimateAndSetImuBiases(const localization_measurements::ImuMeasurement &imu_measurement,
                             const int num_imu_measurements_per_bias_estimate,
                             std::vector<localization_measurements::ImuMeasurement> &imu_bias_measurements,
                             GraphLocInitialization &graph_loc_initialization);

bool ValidPointSet(const std::deque<localization_measurements::FeaturePoint> &points,
                   const double min_avg_distance_from_mean);

geometry_msgs::PoseWithCovarianceStamped LatestPoseMsg(const GraphLocalizer &localization_measurements);

ros::Time RosTimeFromHeader(const std_msgs::Header &header);

localization_measurements::Time TimeFromHeader(const std_msgs::Header &header);

Eigen::Isometry3d EigenPose(const ff_msgs::VisualLandmarks &vl_features, const Eigen::Isometry3d &nav_cam_T_body);

ff_msgs::EkfState EkfStateMsg(const localization_measurements::CombinedNavState &combined_nav_state,
                              const Eigen::Vector3d &acceleration, const Eigen::Vector3d &angular_velocity,
                              const localization_measurements::CombinedNavStateCovariances &covariances);

geometry_msgs::PoseWithCovarianceStamped PoseMsg(const Eigen::Isometry3d &global_T_body,
                                                 const std_msgs::Header &header);
}  // namespace graph_localizer

#endif  // GRAPH_LOCALIZER_UTILITIES_H_