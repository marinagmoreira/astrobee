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

#ifndef DATA_BAGGER_DATA_BAGGER_H_
#define DATA_BAGGER_DATA_BAGGER_H_

#include <boost/date_time/posix_time/posix_time.hpp>

#include <config_reader/config_reader.h>

#include <data_bagger/astrobee_recorder.h>

#include <errno.h>

#include <ff_common/ff_ros.h>
#include <ff_common/ff_names.h>
#include <ff_util/ff_component.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <ff_msgs/msg/data_to_disk_state.hpp>
#include <ff_msgs/msg/data_topics_list.hpp>
#include <ff_msgs/srv/enable_recording.hpp>
#include <ff_msgs/srv/set_data_to_disk.hpp>
namespace ff_msgs {
typedef msg::DataToDiskState DataToDiskState;
typedef msg::DataTopicsList DataTopicsList;
typedef msg::SaveSettings SaveSettings;
typedef srv::EnableRecording EnableRecording;
typedef srv::SetDataToDisk SetDataToDisk;
}  // namespace ff_msgs


#include <string>
#include <vector>


namespace data_bagger {

class DataBagger : public ff_util::FreeFlyerComponent {
 public:
  explicit DataBagger(const rclcpp::NodeOptions & options);
  ~DataBagger();

  // Service that sets delayed recording settings
bool SetDelayedDataToDiskService(const std::shared_ptr<ff_msgs::SetDataToDisk::Request> req,
                                      std::shared_ptr<ff_msgs::SetDataToDisk::Response> res);

  // This service enables and disables the delayed recording
  bool EnableDelayedRecordingService(const std::shared_ptr<ff_msgs::EnableRecording::Request> req,
                                      std::shared_ptr<ff_msgs::EnableRecording::Response> res);

 protected:
  virtual void Initialize(NodeHandle &nh);
  bool ReadParams();  // Reads data bagger parameters and default profile

 private:
  // Reads all topic names published by the robot
  void GetTopicNames();

  // Recursively created bag destionation folder
  bool MakeDir(std::string dir, bool assert_init_fault, std::string &err_msg);

  // Gets the date for name generation
  std::string GetDate(bool with_time);

  // Adds the namespace to topic names
  void AddTopicNamespace(std::string &topic);

  // Timer for when the robot finishes startup, starts immediate recording
  void OnStartupTimer();

  // Sets immediate recording settings
  bool SetImmediateDataToDisk(std::string &err_msg);

  bool SetDelayedDataToDisk(ff_msgs::DataToDiskState &state,
                            std::string &err_msg);

  // Start Recordings
  void StartDelayedRecording();
  void StartImmediateRecording();

  // Clear topics, stop recording
  void ResetRecorders(bool immediate);

  // Update data bagger profile state
  void GenerateCombinedState(ff_msgs::DataToDiskState *ground_state);

  // Publish data bagger state
  void PublishState();

  astrobee_rosbag::Recorder *delayed_recorder_, *immediate_recorder_;

  config_reader::ConfigReader config_params_;

  ff_msgs::DataToDiskState default_data_state_, combined_data_state_;

  int pub_queue_size_;
  unsigned int startup_time_secs_;
  int64_t bag_size_bytes_;

  rclcpp::Publisher<ff_msgs::DataToDiskState>::SharedPtr pub_data_state_;
  rclcpp::Publisher<ff_msgs::DataTopicsList>::SharedPtr pub_data_topics_;
  ff_util::FreeFlyerTimer startup_timer_;

  rclcpp::Service<ff_msgs::SetDataToDisk>::SharedPtr set_service_;
  rclcpp::Service<ff_msgs::EnableRecording>::SharedPtr record_service_;

  rosbag2_storage::StorageOptions storage_options_delayed_;
  rosbag2_storage::StorageOptions storage_options_immediate_;
  rosbag2_transport::RecordOptions record_options_delayed_;
  rosbag2_transport::RecordOptions record_options_immediate_;

  std::thread delayed_thread_, immediate_thread_;
  std::string save_dir_, robot_name_, delayed_profile_name_;
};

}  //  namespace data_bagger

#endif  // DATA_BAGGER_DATA_BAGGER_H_
