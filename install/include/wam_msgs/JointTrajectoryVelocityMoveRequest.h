/* Software License Agreement (BSD License)
 *
 * Copyright (c) 2011, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of Willow Garage, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Auto-generated by genmsg_cpp from file /home/robot/catkin_ws/src/wam_common/wam_msgs/srv/JointTrajectoryVelocityMove.srv
 *
 */


#ifndef WAM_MSGS_MESSAGE_JOINTTRAJECTORYVELOCITYMOVEREQUEST_H
#define WAM_MSGS_MESSAGE_JOINTTRAJECTORYVELOCITYMOVEREQUEST_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <trajectory_msgs/JointTrajectory.h>

namespace wam_msgs
{
template <class ContainerAllocator>
struct JointTrajectoryVelocityMoveRequest_
{
  typedef JointTrajectoryVelocityMoveRequest_<ContainerAllocator> Type;

  JointTrajectoryVelocityMoveRequest_()
    : jointTrajectory()
    , velocity(0.0)
    , length(0.0)  {
    }
  JointTrajectoryVelocityMoveRequest_(const ContainerAllocator& _alloc)
    : jointTrajectory(_alloc)
    , velocity(0.0)
    , length(0.0)  {
    }



   typedef  ::trajectory_msgs::JointTrajectory_<ContainerAllocator>  _jointTrajectory_type;
  _jointTrajectory_type jointTrajectory;

   typedef double _velocity_type;
  _velocity_type velocity;

   typedef double _length_type;
  _length_type length;




  typedef boost::shared_ptr< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;

}; // struct JointTrajectoryVelocityMoveRequest_

typedef ::wam_msgs::JointTrajectoryVelocityMoveRequest_<std::allocator<void> > JointTrajectoryVelocityMoveRequest;

typedef boost::shared_ptr< ::wam_msgs::JointTrajectoryVelocityMoveRequest > JointTrajectoryVelocityMoveRequestPtr;
typedef boost::shared_ptr< ::wam_msgs::JointTrajectoryVelocityMoveRequest const> JointTrajectoryVelocityMoveRequestConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace wam_msgs

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/hydro/share/std_msgs/cmake/../msg'], 'sensor_msgs': ['/opt/ros/hydro/share/sensor_msgs/cmake/../msg'], 'trajectory_msgs': ['/opt/ros/hydro/share/trajectory_msgs/cmake/../msg'], 'geometry_msgs': ['/opt/ros/hydro/share/geometry_msgs/cmake/../msg'], 'wam_msgs': ['/home/robot/catkin_ws/src/wam_common/wam_msgs/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "d32873f39c98c01e043b969aebd32a6a";
  }

  static const char* value(const ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xd32873f39c98c01eULL;
  static const uint64_t static_value2 = 0x043b969aebd32a6aULL;
};

template<class ContainerAllocator>
struct DataType< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "wam_msgs/JointTrajectoryVelocityMoveRequest";
  }

  static const char* value(const ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "trajectory_msgs/JointTrajectory jointTrajectory\n\
float64 velocity\n\
float64 length\n\
\n\
================================================================================\n\
MSG: trajectory_msgs/JointTrajectory\n\
Header header\n\
string[] joint_names\n\
JointTrajectoryPoint[] points\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.secs: seconds (stamp_secs) since epoch\n\
# * stamp.nsecs: nanoseconds since stamp_secs\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
\n\
================================================================================\n\
MSG: trajectory_msgs/JointTrajectoryPoint\n\
# Each trajectory point specifies either positions[, velocities[, accelerations]]\n\
# or positions[, effort] for the trajectory to be executed.\n\
# All specified values are in the same order as the joint names in JointTrajectory.msg\n\
\n\
float64[] positions\n\
float64[] velocities\n\
float64[] accelerations\n\
float64[] effort\n\
duration time_from_start\n\
";
  }

  static const char* value(const ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.jointTrajectory);
      stream.next(m.velocity);
      stream.next(m.length);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct JointTrajectoryVelocityMoveRequest_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::wam_msgs::JointTrajectoryVelocityMoveRequest_<ContainerAllocator>& v)
  {
    s << indent << "jointTrajectory: ";
    s << std::endl;
    Printer< ::trajectory_msgs::JointTrajectory_<ContainerAllocator> >::stream(s, indent + "  ", v.jointTrajectory);
    s << indent << "velocity: ";
    Printer<double>::stream(s, indent + "  ", v.velocity);
    s << indent << "length: ";
    Printer<double>::stream(s, indent + "  ", v.length);
  }
};

} // namespace message_operations
} // namespace ros

#endif // WAM_MSGS_MESSAGE_JOINTTRAJECTORYVELOCITYMOVEREQUEST_H
