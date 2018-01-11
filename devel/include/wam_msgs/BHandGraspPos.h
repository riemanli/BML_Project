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
 * Auto-generated by gensrv_cpp from file /home/robot/rieman_ws/src/wam_common/wam_msgs/srv/BHandGraspPos.srv
 *
 */


#ifndef WAM_MSGS_MESSAGE_BHANDGRASPPOS_H
#define WAM_MSGS_MESSAGE_BHANDGRASPPOS_H

#include <ros/service_traits.h>


#include <wam_msgs/BHandGraspPosRequest.h>
#include <wam_msgs/BHandGraspPosResponse.h>


namespace wam_msgs
{

struct BHandGraspPos
{

typedef BHandGraspPosRequest Request;
typedef BHandGraspPosResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct BHandGraspPos
} // namespace wam_msgs


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::wam_msgs::BHandGraspPos > {
  static const char* value()
  {
    return "718158cfca4f89c6952f799c53697f42";
  }

  static const char* value(const ::wam_msgs::BHandGraspPos&) { return value(); }
};

template<>
struct DataType< ::wam_msgs::BHandGraspPos > {
  static const char* value()
  {
    return "wam_msgs/BHandGraspPos";
  }

  static const char* value(const ::wam_msgs::BHandGraspPos&) { return value(); }
};


// service_traits::MD5Sum< ::wam_msgs::BHandGraspPosRequest> should match 
// service_traits::MD5Sum< ::wam_msgs::BHandGraspPos > 
template<>
struct MD5Sum< ::wam_msgs::BHandGraspPosRequest>
{
  static const char* value()
  {
    return MD5Sum< ::wam_msgs::BHandGraspPos >::value();
  }
  static const char* value(const ::wam_msgs::BHandGraspPosRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::wam_msgs::BHandGraspPosRequest> should match 
// service_traits::DataType< ::wam_msgs::BHandGraspPos > 
template<>
struct DataType< ::wam_msgs::BHandGraspPosRequest>
{
  static const char* value()
  {
    return DataType< ::wam_msgs::BHandGraspPos >::value();
  }
  static const char* value(const ::wam_msgs::BHandGraspPosRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::wam_msgs::BHandGraspPosResponse> should match 
// service_traits::MD5Sum< ::wam_msgs::BHandGraspPos > 
template<>
struct MD5Sum< ::wam_msgs::BHandGraspPosResponse>
{
  static const char* value()
  {
    return MD5Sum< ::wam_msgs::BHandGraspPos >::value();
  }
  static const char* value(const ::wam_msgs::BHandGraspPosResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::wam_msgs::BHandGraspPosResponse> should match 
// service_traits::DataType< ::wam_msgs::BHandGraspPos > 
template<>
struct DataType< ::wam_msgs::BHandGraspPosResponse>
{
  static const char* value()
  {
    return DataType< ::wam_msgs::BHandGraspPos >::value();
  }
  static const char* value(const ::wam_msgs::BHandGraspPosResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // WAM_MSGS_MESSAGE_BHANDGRASPPOS_H
