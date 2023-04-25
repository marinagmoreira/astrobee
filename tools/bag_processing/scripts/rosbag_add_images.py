#!/usr/bin/env python
# Copyright (c) 2017, United States Government, as represented by the
# Administrator of the National Aeronautics and Space Administration.
#
# All rights reserved.
#
# The Astrobee platform is licensed under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with the
# License. You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.
"""
Converts bayer encoded images from the provided bagfile to grayscale and color images in a new bagfile.
"""

import argparse
import os
import shutil
import sys

import cv2
import rosbag
import rospy
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image

import utilities.utilities


def add_image_topic(
    bagfile,
    output_bag_name,
    image_dir,
    add_image_topic,
    info_image_topic,
    save_all_topics=False,
):
    bridge = CvBridge()
    topics = dict((bayer_image_topic.replace("nav", cam), cam) for cam in list_cam)
    output_bag = rosbag.Bag(output_bag_name, "w")
    topics_bag = [] if save_all_topics else info_image_topic

    with rosbag.Bag(bagfile, "r") as bag:
        for topic, msg, t in bag.read_messages(topics_bag):
            if topic == info_image_topic:
                img_path = os.path.join(
                    image_dir,
                    str(msg.header.stamp.secs)
                    + "."
                    + "%03d" % (msg.header.stamp.nsecs * 0.000001)
                    + ".jpg",
                )
                # Make sure image exists
                if not os.path.exists(img_path):
                    raise RuntimeError("Could not find %s" % img_path)

                try:
                    image = cv2.imread(img_path, cv2.IMREAD_COLOR)
                except (CvBridgeError) as e:
                    print(e)
                image_msg = bridge.cv2_to_compressed_imgmsg(image, encoding="bgr8")
                image_msg.header = msg.header
                output_bag.write(add_image_topic, image_msg, t)
            if save_all_topics:
                output_bag.write(topic, msg, t)
    output_bag.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )
    parser.add_argument(
        "inbag",
        nargs="+",
        help="List of bags to convert. If none provided, all bags in the current directory are used.",
    )
    parser.add_argument(
        "-o", "--output", help="path for output bag", default="adder_{inbag}"
    )
    parser.add_argument(
        "-l",
        "--image-dir",
        default="",
        help="Cameras to be converted, default nav, can add dock.",
    )
    parser.add_argument(
        "-t",
        "--add-image-topic",
        default="/hw/cam_sci/compressed",
        help="Bayer image topic name.",
    )
    parser.add_argument(
        "-i",
        "--info-image-topic",
        default="/hw/cam_sci_info",
        help="Output gray image topic.",
    )
    parser.add_argument(
        "-s",
        "--save-all-topics",
        dest="save_all_topics",
        action="store_true",
        help="Save all topics from input bagfile to output bagfile.",
    )
    parser.add_argument(
        "-n",
        dest="do_nothing",
        action="store_true",
        help="Option to not debayer anything and write output",
    )
    args = parser.parse_args()

    inbag_paths = args.inbag if args.inbag is not None else glob.glob("*.bag")

    for inbag_path in inbag_paths:
        # Check if input bag exists
        if not os.path.isfile(inbag_path):
            print(("Bag file " + inbag_path + " does not exist."))
            sys.exit(1)
        output_bag_name = args.output.format(inbag=inbag_path)

        # Check if output bag already exists
        if os.path.exists(output_bag_name):
            parser.error("not replacing existing file %s" % output_bag_name)
        if not args.do_nothing:
            # Conver bayer topic to black/white and color
            add_image_topic(
                inbag_path,
                output_bag_name,
                args.image_dir,
                args.add_image_topic,
                args.info_image_topic,
                args.save_all_topics,
            )
        else:
            os.rename(inbag_path, output_bag_name)
