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

#ifndef IMU_INTEGRATION_UTILITIES_H_
#define IMU_INTEGRATION_UTILITIES_H_

#include <localization_common/combined_nav_state.h>
#include <localization_measurements/imu_measurement.h>

#include <gtsam/inference/Symbol.h>
#include <gtsam/navigation/CombinedImuFactor.h>
#include <gtsam/navigation/ImuBias.h>

#include <Eigen/Core>

#include <vector>

namespace imu_integration {
namespace sym = gtsam::symbol_shorthand;
bool EstimateAndSetImuBiases(const localization_measurements::ImuMeasurement& imu_measurement,
                             const int num_imu_measurements_per_bias_estimate,
                             std::vector<localization_measurements::ImuMeasurement>& imu_bias_measurements,
                             Eigen::Vector3d& accelerometer_bias, Eigen::Vector3d& gyro_bias);

localization_measurements::ImuMeasurement Interpolate(
    const localization_measurements::ImuMeasurement& imu_measurement_a,
    const localization_measurements::ImuMeasurement& imu_measurement_b, const localization_common::Time timestamp);

gtsam::PreintegratedCombinedMeasurements Pim(
    const gtsam::imuBias::ConstantBias& bias,
    const boost::shared_ptr<gtsam::PreintegratedCombinedMeasurements::Params>& params);

void AddMeasurement(const localization_measurements::ImuMeasurement& imu_measurement,
                    localization_common::Time& last_added_imu_measurement_time,
                    gtsam::PreintegratedCombinedMeasurements& pim);

localization_common::CombinedNavState PimPredict(const localization_common::CombinedNavState& combined_nav_state,
                                                 const gtsam::PreintegratedCombinedMeasurements& pim);

gtsam::CombinedImuFactor::shared_ptr MakeCombinedImuFactor(const int key_index_0, const int key_index_1,
                                                           const gtsam::PreintegratedCombinedMeasurements& pim);
}  // namespace imu_integration

#endif  // IMU_INTEGRATION_UTILITIES_H_