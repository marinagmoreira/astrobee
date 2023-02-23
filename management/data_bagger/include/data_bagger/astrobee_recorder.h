/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2008, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of Willow Garage, Inc. nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
********************************************************************/

#ifndef DATA_BAGGER_ASTROBEE_RECORDER_H_
#define DATA_BAGGER_ASTROBEE_RECORDER_H_

#include <sys/stat.h>
#if !defined(_MSC_VER)
  #include <termios.h>
  #include <unistd.h>
#endif
#include <time.h>

#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/regex.hpp>


#include <rosbag2_transport/recorder.hpp>

#include <std_msgs/msg/empty.hpp>
namespace std_msgs {
typedef msg::Empty Empty;
}  // namespace std_msgs
// #include <topic_tools/shape_shifter.h>

#include <queue>
#include <set>
#include <string>
#include <vector>
#include <list>
#include <mutex>
#include <condition_variable> // NOLINT

namespace astrobee_rosbag {

// using rosbag::OutgoingMessage;
// using rosbag::OutgoingQueue;
using rosbag2_transport::RecordOptions;
using rosbag2_storage::StorageOptions;
using rosbag2_cpp::Writer;
using namespace std::chrono_literals;  // NOLINT

class Recorder : public rosbag2_transport::Recorder{
 public:
  explicit Recorder(const StorageOptions & storage_options, const RecordOptions & record_options);
  ~Recorder();

  int record();

  void stop();

 private:
  bool checkLogging();
  bool scheduledCheckDisk();
  bool checkDisk();

  void checkNumSplits();


 private:
    std::shared_ptr<rosbag2_cpp::Writer> writer_;
    StorageOptions                storage_options_;
    RecordOptions                 record_options_;

    std::string                   target_filename_;
    std::string                   write_filename_;
    std::list<std::string>        current_files_;

    std::set<std::string>         currently_recording_;  //!< set of currenly recording topics
    int                           num_subscribers_;      //!< used for book-keeping of our number of subscribers

    int                           exit_code_;            //!< eventual exit code

    boost::condition_variable_any queue_condition_;      //!< conditional variable for queue
    boost::mutex                  queue_mutex_;          //!< mutex for queue
    // std::queue<OutgoingMessage>*  queue_;                //!< queue for storing
    uint64_t                      queue_size_;           //!< queue size
    uint64_t                      max_queue_size_;       //!< max queue size

    uint64_t                      split_count_;          //!< split count

    // std::queue<OutgoingQueue>     queue_queue_;          //!< queue of queues to be used by the snapshot recorders

    rclcpp::Time                     last_buffer_warn_;

    rclcpp::Time                     start_time_;

    bool                          writing_enabled_;
    boost::mutex                  check_disk_mutex_;
    // ros::WallTime                 check_disk_next_;
    // ros::WallTime                 warn_next_;
    // ros::NodeHandle               node_handle_;
    // ros::CallbackQueue            callback_queue_;
    std::unique_ptr<rclcpp::executors::SingleThreadedExecutor> exec_;
};

}  // namespace astrobee_rosbag

#endif  // DATA_BAGGER_ASTROBEE_RECORDER_H_

