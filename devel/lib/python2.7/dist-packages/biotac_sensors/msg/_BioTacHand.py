"""autogenerated by genpy from biotac_sensors/BioTacHand.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import biotac_sensors.msg
import genpy
import std_msgs.msg

class BioTacHand(genpy.Message):
  _md5sum = "d96f900cbfb096d61de125081e11dcab"
  _type = "biotac_sensors/BioTacHand"
  _has_header = True #flag to mark the presence of a Header object
  _full_text = """Header header
string hand_id
biotac_sensors/BioTacData[] bt_data
biotac_sensors/BioTacTime bt_time

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.secs: seconds (stamp_secs) since epoch
# * stamp.nsecs: nanoseconds since stamp_secs
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

================================================================================
MSG: biotac_sensors/BioTacData
string bt_serial
uint16 bt_position
uint16 tdc_data
uint16 tac_data
uint16 pdc_data
uint16[22] pac_data
uint16[19] electrode_data

================================================================================
MSG: biotac_sensors/BioTacTime
time frame_start_time
time frame_end_time
uint32 tdc_ns_offset
uint32 tac_ns_offset
uint32 pdc_ns_offset
uint32[22] pac_ns_offset
uint32[19] electrode_ns_offset

"""
  __slots__ = ['header','hand_id','bt_data','bt_time']
  _slot_types = ['std_msgs/Header','string','biotac_sensors/BioTacData[]','biotac_sensors/BioTacTime']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       header,hand_id,bt_data,bt_time

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(BioTacHand, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.header is None:
        self.header = std_msgs.msg.Header()
      if self.hand_id is None:
        self.hand_id = ''
      if self.bt_data is None:
        self.bt_data = []
      if self.bt_time is None:
        self.bt_time = biotac_sensors.msg.BioTacTime()
    else:
      self.header = std_msgs.msg.Header()
      self.hand_id = ''
      self.bt_data = []
      self.bt_time = biotac_sensors.msg.BioTacTime()

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_struct_3I.pack(_x.header.seq, _x.header.stamp.secs, _x.header.stamp.nsecs))
      _x = self.header.frame_id
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      _x = self.hand_id
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      length = len(self.bt_data)
      buff.write(_struct_I.pack(length))
      for val1 in self.bt_data:
        _x = val1.bt_serial
        length = len(_x)
        if python3 or type(_x) == unicode:
          _x = _x.encode('utf-8')
          length = len(_x)
        buff.write(struct.pack('<I%ss'%length, length, _x))
        _x = val1
        buff.write(_struct_4H.pack(_x.bt_position, _x.tdc_data, _x.tac_data, _x.pdc_data))
        buff.write(_struct_22H.pack(*val1.pac_data))
        buff.write(_struct_19H.pack(*val1.electrode_data))
      _x = self
      buff.write(_struct_7I.pack(_x.bt_time.frame_start_time.secs, _x.bt_time.frame_start_time.nsecs, _x.bt_time.frame_end_time.secs, _x.bt_time.frame_end_time.nsecs, _x.bt_time.tdc_ns_offset, _x.bt_time.tac_ns_offset, _x.bt_time.pdc_ns_offset))
      buff.write(_struct_22I.pack(*self.bt_time.pac_ns_offset))
      buff.write(_struct_19I.pack(*self.bt_time.electrode_ns_offset))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      if self.header is None:
        self.header = std_msgs.msg.Header()
      if self.bt_data is None:
        self.bt_data = None
      if self.bt_time is None:
        self.bt_time = biotac_sensors.msg.BioTacTime()
      end = 0
      _x = self
      start = end
      end += 12
      (_x.header.seq, _x.header.stamp.secs, _x.header.stamp.nsecs,) = _struct_3I.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.header.frame_id = str[start:end].decode('utf-8')
      else:
        self.header.frame_id = str[start:end]
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.hand_id = str[start:end].decode('utf-8')
      else:
        self.hand_id = str[start:end]
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.bt_data = []
      for i in range(0, length):
        val1 = biotac_sensors.msg.BioTacData()
        start = end
        end += 4
        (length,) = _struct_I.unpack(str[start:end])
        start = end
        end += length
        if python3:
          val1.bt_serial = str[start:end].decode('utf-8')
        else:
          val1.bt_serial = str[start:end]
        _x = val1
        start = end
        end += 8
        (_x.bt_position, _x.tdc_data, _x.tac_data, _x.pdc_data,) = _struct_4H.unpack(str[start:end])
        start = end
        end += 44
        val1.pac_data = _struct_22H.unpack(str[start:end])
        start = end
        end += 38
        val1.electrode_data = _struct_19H.unpack(str[start:end])
        self.bt_data.append(val1)
      _x = self
      start = end
      end += 28
      (_x.bt_time.frame_start_time.secs, _x.bt_time.frame_start_time.nsecs, _x.bt_time.frame_end_time.secs, _x.bt_time.frame_end_time.nsecs, _x.bt_time.tdc_ns_offset, _x.bt_time.tac_ns_offset, _x.bt_time.pdc_ns_offset,) = _struct_7I.unpack(str[start:end])
      start = end
      end += 88
      self.bt_time.pac_ns_offset = _struct_22I.unpack(str[start:end])
      start = end
      end += 76
      self.bt_time.electrode_ns_offset = _struct_19I.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_struct_3I.pack(_x.header.seq, _x.header.stamp.secs, _x.header.stamp.nsecs))
      _x = self.header.frame_id
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      _x = self.hand_id
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      length = len(self.bt_data)
      buff.write(_struct_I.pack(length))
      for val1 in self.bt_data:
        _x = val1.bt_serial
        length = len(_x)
        if python3 or type(_x) == unicode:
          _x = _x.encode('utf-8')
          length = len(_x)
        buff.write(struct.pack('<I%ss'%length, length, _x))
        _x = val1
        buff.write(_struct_4H.pack(_x.bt_position, _x.tdc_data, _x.tac_data, _x.pdc_data))
        buff.write(val1.pac_data.tostring())
        buff.write(val1.electrode_data.tostring())
      _x = self
      buff.write(_struct_7I.pack(_x.bt_time.frame_start_time.secs, _x.bt_time.frame_start_time.nsecs, _x.bt_time.frame_end_time.secs, _x.bt_time.frame_end_time.nsecs, _x.bt_time.tdc_ns_offset, _x.bt_time.tac_ns_offset, _x.bt_time.pdc_ns_offset))
      buff.write(self.bt_time.pac_ns_offset.tostring())
      buff.write(self.bt_time.electrode_ns_offset.tostring())
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      if self.header is None:
        self.header = std_msgs.msg.Header()
      if self.bt_data is None:
        self.bt_data = None
      if self.bt_time is None:
        self.bt_time = biotac_sensors.msg.BioTacTime()
      end = 0
      _x = self
      start = end
      end += 12
      (_x.header.seq, _x.header.stamp.secs, _x.header.stamp.nsecs,) = _struct_3I.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.header.frame_id = str[start:end].decode('utf-8')
      else:
        self.header.frame_id = str[start:end]
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.hand_id = str[start:end].decode('utf-8')
      else:
        self.hand_id = str[start:end]
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.bt_data = []
      for i in range(0, length):
        val1 = biotac_sensors.msg.BioTacData()
        start = end
        end += 4
        (length,) = _struct_I.unpack(str[start:end])
        start = end
        end += length
        if python3:
          val1.bt_serial = str[start:end].decode('utf-8')
        else:
          val1.bt_serial = str[start:end]
        _x = val1
        start = end
        end += 8
        (_x.bt_position, _x.tdc_data, _x.tac_data, _x.pdc_data,) = _struct_4H.unpack(str[start:end])
        start = end
        end += 44
        val1.pac_data = numpy.frombuffer(str[start:end], dtype=numpy.uint16, count=22)
        start = end
        end += 38
        val1.electrode_data = numpy.frombuffer(str[start:end], dtype=numpy.uint16, count=19)
        self.bt_data.append(val1)
      _x = self
      start = end
      end += 28
      (_x.bt_time.frame_start_time.secs, _x.bt_time.frame_start_time.nsecs, _x.bt_time.frame_end_time.secs, _x.bt_time.frame_end_time.nsecs, _x.bt_time.tdc_ns_offset, _x.bt_time.tac_ns_offset, _x.bt_time.pdc_ns_offset,) = _struct_7I.unpack(str[start:end])
      start = end
      end += 88
      self.bt_time.pac_ns_offset = numpy.frombuffer(str[start:end], dtype=numpy.uint32, count=22)
      start = end
      end += 76
      self.bt_time.electrode_ns_offset = numpy.frombuffer(str[start:end], dtype=numpy.uint32, count=19)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_22I = struct.Struct("<22I")
_struct_22H = struct.Struct("<22H")
_struct_7I = struct.Struct("<7I")
_struct_3I = struct.Struct("<3I")
_struct_19I = struct.Struct("<19I")
_struct_19H = struct.Struct("<19H")
_struct_4H = struct.Struct("<4H")
