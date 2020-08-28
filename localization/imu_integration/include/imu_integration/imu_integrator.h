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

#ifndef IMU_INTEGRATION_IMU_INTEGRATOR_H_
#define IMU_INTEGRATION_IMU_INTEGRATOR_H_

#include <localization_common/combined_nav_state.h>
#include <localization_common/time.h>
#include <localization_measurements/imu_measurement.h>

#include <gtsam/navigation/CombinedImuFactor.h>
#include <gtsam/navigation/ImuBias.h>

#include <Eigen/Core>

#include <map>

namespace imu_integration {
// Integrates imu measurements and propagates uncertainties.
// Maintains a window of measurements so that any interval of measurements in
// that window can be integrated into a pim.
class ImuIntegrator {
 public:
  ImuIntegrator(const Eigen::Isometry3d& body_T_imu, const Eigen::Vector3d& gravity);

  // Buffers imu measurement so they can be integrated when needed.
  // Delayed integration useful so imu integation does not advance
  // past latest sensor measurement timestamps.
  void BufferImuMeasurement(const localization_measurements::ImuMeasurement& imu_measurement);

  // Returns last integrated measurement timestamp.
  boost::optional<localization_common::Time> IntegrateImuMeasurements(
      const localization_common::Time start_time, const localization_common::Time end_time,
      gtsam::PreintegratedCombinedMeasurements& pim) const;

  boost::optional<gtsam::PreintegratedCombinedMeasurements> IntegratedPim(
      const gtsam::imuBias::ConstantBias& bias, const localization_common::Time start_time,
      const localization_common::Time end_time,
      boost::shared_ptr<gtsam::PreintegratedCombinedMeasurements::Params> params) const;

  void RemoveOldMeasurements(const localization_common::Time new_start_time);

  boost::optional<localization_common::Time> OldestTime() const;

  boost::optional<localization_common::Time> LatestTime() const;

  boost::optional<localization_measurements::ImuMeasurement> LatestMeasurement() const;

  bool Empty() const;

  int Size() const;

  boost::shared_ptr<gtsam::PreintegratedCombinedMeasurements::Params> pim_params() const;

 private:
  boost::shared_ptr<gtsam::PreintegratedCombinedMeasurements::Params> pim_params_;
  std::map<localization_common::Time, localization_measurements::ImuMeasurement> measurements_;
  // Static calibration
  Eigen::Isometry3d body_T_imu_;
  // From gtsam: Realistic MEMS white noise characteristics. Angular and
  // velocity random walk expressed in degrees respectively m/s per sqrt(hr).
  static constexpr double kGyroSigma_ = 0.00001;    // (0.5 * M_PI / 180) / 60;  // 0.5 degree ARW
  static constexpr double kAccelSigma_ = 0.000001;  // 0.1 / 60; // 10 cm VRW
  // TODO(rsoussan): tune these
  static constexpr double kAccelBiasSigma_ = 0.0001;
  static constexpr double kGyroBiasSigma_ = 0.0001;
};
}  // namespace imu_integration

#endif  // IMU_INTEGRATION_IMU_INTEGRATOR_H_