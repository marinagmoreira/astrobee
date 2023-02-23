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

#include "data_bagger/astrobee_recorder.h"

#include <sys/stat.h>
#include <boost/filesystem.hpp>
// Boost filesystem v3 is default in 1.46.0 and above
// Fallback to original posix code (*nix only) if this is not true
#if BOOST_FILESYSTEM_VERSION < 3
  #include <sys/statvfs.h>
#endif
#include <time.h>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/date_time/local_time/local_time.hpp>

// #include <topic_tools/shape_shifter.h>

#include <queue>
#include <set>
#include <sstream>
#include <string>

#define foreach BOOST_FOREACH

using std::cout;
using std::endl;
using std::set;
using std::string;
using std::vector;
using boost::shared_ptr;
// using ros::Time;

// using rosbag::OutgoingMessage;
// using rosbag::OutgoingQueue;
// using rosbag::RecorderOptions;
// using rosbag::Bag;
// using rosbag::BagException;

namespace astrobee_rosbag {
// Recorder

Recorder::Recorder(const StorageOptions & storage_options, const RecordOptions & record_options) :
    storage_options_(storage_options),
    record_options_(record_options),
    rosbag2_transport::Recorder(writer_, storage_options, record_options) {
  exec_ = std::make_unique<rclcpp::executors::SingleThreadedExecutor>();
    std::signal(
      SIGTERM, [](int /* signal */) {
        rclcpp::shutdown();
      });
}
Recorder::~Recorder() {
}

int Recorder::record() {
  // // Make sure topics are specified
  // if (record_options_.topics.size() == 0 && !record_options_.all) {
  //     fprintf(stderr, "No topics specified.\n");
  //     return 1;

  // }
  if (record_options_.rmw_serialization_format.empty()) {
    record_options_.rmw_serialization_format = std::string(rmw_get_serialization_format());
  }

  auto recorder = std::make_shared<rosbag2_transport::Recorder>(
    std::move(writer_), storage_options_, record_options_);
    recorder->record();

    exec_->add_node(recorder);
    exec_->spin();

    return 0;
}

void Recorder::stop() {
    exec_->cancel();
    // return exit_code_;
}

void Recorder::checkNumSplits() {
    // if (options_.max_splits > 0) {
    //     current_files_.push_back(target_filename_);
    //     if (current_files_.size() > options_.max_splits) {
    //         int err = unlink(current_files_.front().c_str());
    //         if (err != 0) {
    //             ROS_ERROR("Unable to remove %s: %s", current_files_.front().c_str(), strerror(errno));
    //         }
    //         current_files_.pop_front();
    //     }
    // }
}


bool Recorder::scheduledCheckDisk() {
    // boost::mutex::scoped_lock lock(check_disk_mutex_);

    // if (ros::WallTime::now() < check_disk_next_)
    //     return true;

    // check_disk_next_ += ros::WallDuration().fromSec(20.0);
    return checkDisk();
}

bool Recorder::checkDisk() {
// #if BOOST_FILESYSTEM_VERSION < 3
//     struct statvfs fiData;
//     if ((statvfs(bag_.getFileName().c_str(), &fiData)) < 0) {
//         ROS_WARN("Failed to check filesystem stats.");
//         return true;
//     }
//     uint64_t free_space = 0;
//     free_space = (uint64_t) (fiData.f_bsize) * (uint64_t) (fiData.f_bavail);
//     if (free_space < options_.min_space) {
//         ROS_ERROR("Less than %s of space free on disk with %s.  Disabling recording.",
//                    options_.min_space_str.c_str(), bag_.getFileName().c_str());
//         writing_enabled_ = false;
//         return false;
//     } else if (free_space < 5 * options_.min_space) {
//         ROS_WARN("Less than 5 x %s of space free on disk with %s.",
//                   options_.min_space_str.c_str(), bag_.getFileName().c_str());
//     } else {
//         writing_enabled_ = true;
//     }
// #else
//     boost::filesystem::path p(boost::filesystem::system_complete(bag_.getFileName().c_str()));
//     p = p.parent_path();
//     boost::filesystem::space_info info;
//     try {
//         info = boost::filesystem::space(p);
//     }
//     catch (boost::filesystem::filesystem_error &e) {
//         ROS_WARN("Failed to check filesystem stats [%s].", e.what());
//         writing_enabled_ = false;
//         return false;
//     }
//     if (info.available < options_.min_space) {
//         ROS_ERROR("Less than %s of space free on disk with %s.  Disabling recording.",
//                    options_.min_space_str.c_str(), bag_.getFileName().c_str());
//         writing_enabled_ = false;
//         return false;
//     } else if (info.available < 5 * options_.min_space) {
//         ROS_WARN("Less than 5 x %s of space free on disk with %s.",
//                   options_.min_space_str.c_str(), bag_.getFileName().c_str());
//         writing_enabled_ = true;
//     } else {
//         writing_enabled_ = true;
//     }
// #endif
    return true;
}

bool Recorder::checkLogging() {
  if (writing_enabled_)
      return true;

    // ros::WallTime now = ros::WallTime::now();
    // if (now >= warn_next_) {
    //     warn_next_ = now + ros::WallDuration().fromSec(5.0);
    //     ROS_WARN("Not logging message because logging disabled.  Most likely cause is a full disk.");
    // }
    // return false;
}

}  // namespace astrobee_rosbag

