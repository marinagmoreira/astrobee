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

// Header file must go in src directory for Qt/Rviz plugin
#ifndef LOCALIZATION_RVIZ_PLUGINS_LOCALIZATION_GRAPH_DISPLAY_H_  // NOLINT
#define LOCALIZATION_RVIZ_PLUGINS_LOCALIZATION_GRAPH_DISPLAY_H_  // NOLINT

// Required for Qt
#ifndef Q_MOC_RUN
#include <ff_msgs/LocalizationGraph.h>
#include <graph_localizer/graph_localizer.h>
#include <gtsam/geometry/Pose3.h>
#include <gtsam/navigation/CombinedImuFactor.h>
#include <rviz/message_filter_display.h>
#include <rviz/ogre_helpers/arrow.h>
#include <rviz/ogre_helpers/axes.h>
#include <rviz/properties/float_property.h>
#include <vector>
#endif

// Forward declarations for ogre and rviz
namespace Ogre {
class SceneNode;
}

namespace localization_rviz_plugins {

class LocalizationGraphDisplay : public rviz::MessageFilterDisplay<ff_msgs::LocalizationGraph> {
  Q_OBJECT      // NOLINT
      public :  // NOLINT
                LocalizationGraphDisplay();
  ~LocalizationGraphDisplay() = default;

  // private:
 protected:
  void onInitialize() final;
  void reset() final;

 private Q_SLOTS:  // NOLINT

 private:
  void processMessage(const ff_msgs::LocalizationGraph::ConstPtr& graph_msg);
  void clearDisplay();
  void addImuVisual(const graph_localizer::GraphLocalizer& graph_localizer,
                    const gtsam::CombinedImuFactor* const imu_factor);

  std::vector<std::unique_ptr<rviz::Axes>> graph_pose_axes_;
  std::vector<std::unique_ptr<rviz::Arrow>> imu_factor_arrows_;
  std::unique_ptr<rviz::BoolProperty> show_pose_axes_;
  std::unique_ptr<rviz::FloatProperty> pose_axes_size_;
  std::unique_ptr<rviz::BoolProperty> show_imu_factor_arrows_;
  std::unique_ptr<rviz::FloatProperty> imu_factor_arrows_diameter_;
};
}  // namespace localization_rviz_plugins
#endif  // LOCALIZATION_RVIZ_PLUGINS_LOCALIZATION_GRAPH_DISPLAY_H_ NOLINT