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
#ifndef GRAPH_LOCALIZER_GRAPH_LOC_INITIALIZATION_H_
#define GRAPH_LOCALIZER_GRAPH_LOC_INITIALIZATION_H_

#include <camera/camera_params.h>
#include <config_reader/config_reader.h>
#include <graph_localizer/graph_localizer_params.h>
#include <msg_conversions/msg_conversions.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <string>

namespace graph_localizer {
class GraphLocInitialization {
 public:
  void SetBiases(const Eigen::Vector3d &accelerometer_bias, const Eigen::Vector3d &gyro_bias);
  void SetStartPose(const Eigen::Isometry3d &global_T_body_start, const double timestamp);
  void SetCalibration(const Eigen::Isometry3d &body_T_imu, const Eigen::Isometry3d &body_T_nav_cam,
                      const Eigen::Matrix3d &nav_cam_intrinsics, const Eigen::Isometry3d &body_T_dock_cam,
                      const Eigen::Matrix3d &dock_cam_intrinsics, const Eigen::Isometry3d &world_T_dock,
                      const Eigen::Vector3d &gravity);
  bool ReadyToInitialize() const;
  void ResetBiasesAndStartPose();
  void ResetStartPose();
  void ResetBiases();
  void StartBiasEstimation();
  bool HasBiases() const;
  bool HasStartPose() const;
  bool HasCalibration() const;
  bool HasParams() const;
  bool EstimateBiases() const;
  const GraphLocalizerParams &params() const;
  void LoadSensorParams(config_reader::ConfigReader &config);
  void LoadGraphLocalizerParams(config_reader::ConfigReader &config);

 private:
  bool has_calibration_ = false;
  bool has_biases_ = false;
  bool has_start_pose_ = false;
  bool has_params_ = false;
  bool estimate_biases_ = false;
  graph_localizer::GraphLocalizerParams params_;
};
}  // namespace graph_localizer

#endif  // GRAPH_LOCALIZER_GRAPH_LOC_INITIALIZATION_H_