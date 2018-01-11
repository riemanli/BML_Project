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
 * Auto-generated by gensrv_cpp from file /home/robot/rieman_ws/src/wam_common/wam_msgs/srv/GravityComp.srv
 *
 */


#ifndef WAM_MSGS_MESSAGE_GRAVITYCOMP_H
#define WAM_MSGS_MESSAGE_GRAVITYCOMP_H

#include <ros/service_traits.h>


#include <wam_msgs/GravityCompRequest.h>
#include <wam_msgs/GravityCompResponse.h>


namespace wam_msgs
{

struct GravityComp
{

typedef GravityCompRequest Request;
typedef GravityCompResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct GravityComp
} // namespace wam_msgs


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::wam_msgs::GravityComp > {
  static const char* value()
  {
    return "f83a9fe078fab9865d863fbf215e1c14";
  }

  static const char* value(const ::wam_msgs::GravityComp&) { return value(); }
};

template<>
struct DataType< ::wam_msgs::GravityComp > {
  static const char* value()
  {
    return "wam_msgs/GravityComp";
  }

  static const char* value(const ::wam_msgs::GravityComp&) { return value(); }
};


// service_traits::MD5Sum< ::wam_msgs::GravityCompRequest> should match 
// service_traits::MD5Sum< ::wam_msgs::GravityComp > 
template<>
struct MD5Sum< ::wam_msgs::GravityCompRequest>
{
  static const char* value()
  {
    return MD5Sum< ::wam_msgs::GravityComp >::value();
  }
  static const char* value(const ::wam_msgs::GravityCompRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::wam_msgs::GravityCompRequest> should match 
// service_traits::DataType< ::wam_msgs::GravityComp > 
template<>
struct DataType< ::wam_msgs::GravityCompRequest>
{
  static const char* value()
  {
    return DataType< ::wam_msgs::GravityComp >::value();
  }
  static const char* value(const ::wam_msgs::GravityCompRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::wam_msgs::GravityCompResponse> should match 
// service_traits::MD5Sum< ::wam_msgs::GravityComp > 
template<>
struct MD5Sum< ::wam_msgs::GravityCompResponse>
{
  static const char* value()
  {
    return MD5Sum< ::wam_msgs::GravityComp >::value();
  }
  static const char* value(const ::wam_msgs::GravityCompResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::wam_msgs::GravityCompResponse> should match 
// service_traits::DataType< ::wam_msgs::GravityComp > 
template<>
struct DataType< ::wam_msgs::GravityCompResponse>
{
  static const char* value()
  {
    return DataType< ::wam_msgs::GravityComp >::value();
  }
  static const char* value(const ::wam_msgs::GravityCompResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // WAM_MSGS_MESSAGE_GRAVITYCOMP_H