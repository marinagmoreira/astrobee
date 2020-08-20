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
#ifndef GRAPH_LOCALIZER_GRAPH_LOCALIZER_WRAPPER_H_
#define GRAPH_LOCALIZER_GRAPH_LOCALIZER_WRAPPER_H_

#include <ff_msgs/Feature2dArray.h>
#include <ff_msgs/VisualLandmarks.h>
#include <graph_localizer/graph_loc_initialization.h>
#include <graph_localizer/graph_localizer.h>
#include <localization_measurements/imu_measurement.h>
#include <localization_measurements/matched_projections_measurement.h>

#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <sensor_msgs/Imu.h>

#include <string>
#include <vector>

namespace graph_localizer {
// Handles initialization of parameters, biases, and initial pose for graph
// localizer.  Provides callbacks that can be used by a ROS or non-ROS system
// (i.e. graph_bag, which does not use a ROS core, vs. graph_localizer_nodelet,
// which is used when running live).
class GraphLocalizerWrapper {
 public:
  GraphLocalizerWrapper();

  // Assumes previous bias estimates are available and uses these.
  void ResetLocalizer();

  void ResetBiasesAndLocalizer();

  bool LatestPoseMsg(geometry_msgs::PoseWithCovarianceStamped &latest_pose_msg) const;

  void OpticalFlowCallback(const ff_msgs::Feature2dArray &feature_array_msg);

  void VLVisualLandmarksCallback(const ff_msgs::VisualLandmarks &visual_landmarks_msg);

  void ARVisualLandmarksCallback(const ff_msgs::VisualLandmarks &visual_landmarks_msg);

  void ImuCallback(const sensor_msgs::Imu &imu_msg);

  const FeatureTrackMap *const feature_tracks() const;

 private:
  void InitializeGraph();

  std::unique_ptr<GraphLocalizer> graph_localizer_;
  std::vector<localization_measurements::ImuMeasurement> imu_bias_measurements_;
  int num_bias_estimation_measurements_;
  // TODO(rsoussan): put these somewhere else? make accessor that returns false
  // if latest imu measurement not available?
  Eigen::Vector3d latest_accelerometer_bias_;
  Eigen::Vector3d latest_gyro_bias_;
  localization_measurements::Time latest_bias_timestamp_;
  bool have_latest_imu_biases_;
  // TODO(rsoussan): rename this!
  GraphLocInitialization graph_loc_initialization_;
};
}  // namespace graph_localizer

#endif  // GRAPH_LOCALIZER_GRAPH_LOCALIZER_WRAPPER_H_