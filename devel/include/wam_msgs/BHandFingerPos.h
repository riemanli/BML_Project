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
 * Auto-generated by gensrv_cpp from file /home/robot/rieman_ws/src/wam_common/wam_msgs/srv/BHandFingerPos.srv
 *
 */


#ifndef WAM_MSGS_MESSAGE_BHANDFINGERPOS_H
#define WAM_MSGS_MESSAGE_BHANDFINGERPOS_H

#include <ros/service_traits.h>


#include <wam_msgs/BHandFingerPosRequest.h>
#include <wam_msgs/BHandFingerPosResponse.h>


namespace wam_msgs
{

struct BHandFingerPos
{

typedef BHandFingerPosRequest Request;
typedef BHandFingerPosResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct BHandFingerPos
} // namespace wam_msgs


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::wam_msgs::BHandFingerPos > {
  static const char* value()
  {
    return "b8c122f38450d0302ba1a30c9c588995";
  }

  static const char* value(const ::wam_msgs::BHandFingerPos&) { return value(); }
};

template<>
struct DataType< ::wam_msgs::BHandFingerPos > {
  static const char* value()
  {
    return "wam_msgs/BHandFingerPos";
  }

  static const char* value(const ::wam_msgs::BHandFingerPos&) { return value(); }
};


// service_traits::MD5Sum< ::wam_msgs::BHandFingerPosRequest> should match 
// service_traits::MD5Sum< ::wam_msgs::BHandFingerPos > 
template<>
struct MD5Sum< ::wam_msgs::BHandFingerPosRequest>
{
  static const char* value()
  {
    return MD5Sum< ::wam_msgs::BHandFingerPos >::value();
  }
  static const char* value(const ::wam_msgs::BHandFingerPosRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::wam_msgs::BHandFingerPosRequest> should match 
// service_traits::DataType< ::wam_msgs::BHandFingerPos > 
template<>
struct DataType< ::wam_msgs::BHandFingerPosRequest>
{
  static const char* value()
  {
    return DataType< ::wam_msgs::BHandFingerPos >::value();
  }
  static const char* value(const ::wam_msgs::BHandFingerPosRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::wam_msgs::BHandFingerPosResponse> should match 
// service_traits::MD5Sum< ::wam_msgs::BHandFingerPos > 
template<>
struct MD5Sum< ::wam_msgs::BHandFingerPosResponse>
{
  static const char* value()
  {
    return MD5Sum< ::wam_msgs::BHandFingerPos >::value();
  }
  static const char* value(const ::wam_msgs::BHandFingerPosResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::wam_msgs::BHandFingerPosResponse> should match 
// service_traits::DataType< ::wam_msgs::BHandFingerPos > 
template<>
struct DataType< ::wam_msgs::BHandFingerPosResponse>
{
  static const char* value()
  {
    return DataType< ::wam_msgs::BHandFingerPos >::value();
  }
  static const char* value(const ::wam_msgs::BHandFingerPosResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // WAM_MSGS_MESSAGE_BHANDFINGERPOS_H
