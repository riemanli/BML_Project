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
 * Auto-generated by genmsg_cpp from file /home/robot/rieman_ws/src/biotac_driver/biotac_sensors/msg/BioTacData.msg
 *
 */


#ifndef BIOTAC_SENSORS_MESSAGE_BIOTACDATA_H
#define BIOTAC_SENSORS_MESSAGE_BIOTACDATA_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace biotac_sensors
{
template <class ContainerAllocator>
struct BioTacData_
{
  typedef BioTacData_<ContainerAllocator> Type;

  BioTacData_()
    : bt_serial()
    , bt_position(0)
    , tdc_data(0)
    , tac_data(0)
    , pdc_data(0)
    , pac_data()
    , electrode_data()  {
      pac_data.assign(0);

      electrode_data.assign(0);
  }
  BioTacData_(const ContainerAllocator& _alloc)
    : bt_serial(_alloc)
    , bt_position(0)
    , tdc_data(0)
    , tac_data(0)
    , pdc_data(0)
    , pac_data()
    , electrode_data()  {
      pac_data.assign(0);

      electrode_data.assign(0);
  }



   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _bt_serial_type;
  _bt_serial_type bt_serial;

   typedef uint16_t _bt_position_type;
  _bt_position_type bt_position;

   typedef uint16_t _tdc_data_type;
  _tdc_data_type tdc_data;

   typedef uint16_t _tac_data_type;
  _tac_data_type tac_data;

   typedef uint16_t _pdc_data_type;
  _pdc_data_type pdc_data;

   typedef boost::array<uint16_t, 22>  _pac_data_type;
  _pac_data_type pac_data;

   typedef boost::array<uint16_t, 19>  _electrode_data_type;
  _electrode_data_type electrode_data;




  typedef boost::shared_ptr< ::biotac_sensors::BioTacData_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::biotac_sensors::BioTacData_<ContainerAllocator> const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;

}; // struct BioTacData_

typedef ::biotac_sensors::BioTacData_<std::allocator<void> > BioTacData;

typedef boost::shared_ptr< ::biotac_sensors::BioTacData > BioTacDataPtr;
typedef boost::shared_ptr< ::biotac_sensors::BioTacData const> BioTacDataConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::biotac_sensors::BioTacData_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::biotac_sensors::BioTacData_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace biotac_sensors

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/hydro/share/std_msgs/cmake/../msg'], 'biotac_sensors': ['/home/robot/rieman_ws/src/biotac_driver/biotac_sensors/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::biotac_sensors::BioTacData_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::biotac_sensors::BioTacData_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::biotac_sensors::BioTacData_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::biotac_sensors::BioTacData_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::biotac_sensors::BioTacData_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::biotac_sensors::BioTacData_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::biotac_sensors::BioTacData_<ContainerAllocator> >
{
  static const char* value()
  {
    return "23cbcbcbf4470cc3c87ecb98037f4bb0";
  }

  static const char* value(const ::biotac_sensors::BioTacData_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x23cbcbcbf4470cc3ULL;
  static const uint64_t static_value2 = 0xc87ecb98037f4bb0ULL;
};

template<class ContainerAllocator>
struct DataType< ::biotac_sensors::BioTacData_<ContainerAllocator> >
{
  static const char* value()
  {
    return "biotac_sensors/BioTacData";
  }

  static const char* value(const ::biotac_sensors::BioTacData_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::biotac_sensors::BioTacData_<ContainerAllocator> >
{
  static const char* value()
  {
    return "string bt_serial\n\
uint16 bt_position\n\
uint16 tdc_data\n\
uint16 tac_data\n\
uint16 pdc_data\n\
uint16[22] pac_data\n\
uint16[19] electrode_data\n\
";
  }

  static const char* value(const ::biotac_sensors::BioTacData_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::biotac_sensors::BioTacData_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.bt_serial);
      stream.next(m.bt_position);
      stream.next(m.tdc_data);
      stream.next(m.tac_data);
      stream.next(m.pdc_data);
      stream.next(m.pac_data);
      stream.next(m.electrode_data);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct BioTacData_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::biotac_sensors::BioTacData_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::biotac_sensors::BioTacData_<ContainerAllocator>& v)
  {
    s << indent << "bt_serial: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.bt_serial);
    s << indent << "bt_position: ";
    Printer<uint16_t>::stream(s, indent + "  ", v.bt_position);
    s << indent << "tdc_data: ";
    Printer<uint16_t>::stream(s, indent + "  ", v.tdc_data);
    s << indent << "tac_data: ";
    Printer<uint16_t>::stream(s, indent + "  ", v.tac_data);
    s << indent << "pdc_data: ";
    Printer<uint16_t>::stream(s, indent + "  ", v.pdc_data);
    s << indent << "pac_data[]" << std::endl;
    for (size_t i = 0; i < v.pac_data.size(); ++i)
    {
      s << indent << "  pac_data[" << i << "]: ";
      Printer<uint16_t>::stream(s, indent + "  ", v.pac_data[i]);
    }
    s << indent << "electrode_data[]" << std::endl;
    for (size_t i = 0; i < v.electrode_data.size(); ++i)
    {
      s << indent << "  electrode_data[" << i << "]: ";
      Printer<uint16_t>::stream(s, indent + "  ", v.electrode_data[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // BIOTAC_SENSORS_MESSAGE_BIOTACDATA_H
