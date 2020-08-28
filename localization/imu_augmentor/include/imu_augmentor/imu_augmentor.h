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

#ifndef IMU_AUGMENTOR_IMU_AUGMENTOR_H_
#define IMU_AUGMENTOR_IMU_AUGMENTOR_H_

#include <imu_integration/imu_integrator.h>
#include <localization_common/combined_nav_state.h>
#include <localization_common/time.h>
#include <localization_measurements/imu_measurement.h>

#include <gtsam/navigation/CombinedImuFactor.h>
#include <gtsam/navigation/ImuBias.h>

#include <Eigen/Core>

namespace imu_augmentor {
class ImuAugmentor {
 public:
  ImuAugmentor(const Eigen::Isometry3d& body_T_imu, const Eigen::Vector3d& gravity);

  void BufferImuMeasurement(const localization_measurements::ImuMeasurement& imu_measurement);

  boost::optional<localization_common::CombinedNavState> PimPredict(
      const localization_common::CombinedNavState& combined_nav_state);

  boost::optional<localization_measurements::ImuMeasurement> LatestMeasurement() const;

 private:
  void RemoveOldMeasurements(const localization_common::Time new_start_time);

  imu_integration::ImuIntegrator imu_integrator_;
};
}  // namespace imu_augmentor

#endif  // IMU_AUGMENTOR_IMU_AUGMENTOR_H_