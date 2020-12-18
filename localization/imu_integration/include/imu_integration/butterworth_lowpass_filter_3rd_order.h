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

#ifndef IMU_INTEGRATION_BUTTERWORTH_LOWPASS_FILTER_3RD_ORDER_H_
#define IMU_INTEGRATION_BUTTERWORTH_LOWPASS_FILTER_3RD_ORDER_H_

#include <imu_integration/filter.h>

#include <localization_common/logger.h>

#include <array>

namespace imu_integration {
class ButterworthLowpassFilter3rdOrder : public Filter {
 public:
  ButterworthLowpassFilter3rdOrder();
  // Returns filtered value and timestamp
  double AddValue(const double value) final;

 private:
  void Initialize(const double first_value, const double gain);
  // Notation taken from mkfilter site
  // /www/usr/fisher/helpers/mkfilter
  std::array<double, 6> xv_;
  std::array<double, 6> yv_;
  bool initialized_;
};
}  // namespace imu_integration

#endif  // IMU_INTEGRATION_BUTTERWORTH_LOWPASS_FILTER_3RD_ORDER_H_
