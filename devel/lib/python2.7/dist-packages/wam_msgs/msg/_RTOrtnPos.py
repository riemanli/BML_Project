"""autogenerated by genpy from wam_msgs/RTOrtnPos.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class RTOrtnPos(genpy.Message):
  _md5sum = "5b352081db430205c87e0e439f09a0b2"
  _type = "wam_msgs/RTOrtnPos"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """float32[4] orientation
float32[4] rate_limits

"""
  __slots__ = ['orientation','rate_limits']
  _slot_types = ['float32[4]','float32[4]']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       orientation,rate_limits

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(RTOrtnPos, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.orientation is None:
        self.orientation = [0.,0.,0.,0.]
      if self.rate_limits is None:
        self.rate_limits = [0.,0.,0.,0.]
    else:
      self.orientation = [0.,0.,0.,0.]
      self.rate_limits = [0.,0.,0.,0.]

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
      buff.write(_struct_4f.pack(*self.orientation))
      buff.write(_struct_4f.pack(*self.rate_limits))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      end = 0
      start = end
      end += 16
      self.orientation = _struct_4f.unpack(str[start:end])
      start = end
      end += 16
      self.rate_limits = _struct_4f.unpack(str[start:end])
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
      buff.write(self.orientation.tostring())
      buff.write(self.rate_limits.tostring())
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      end = 0
      start = end
      end += 16
      self.orientation = numpy.frombuffer(str[start:end], dtype=numpy.float32, count=4)
      start = end
      end += 16
      self.rate_limits = numpy.frombuffer(str[start:end], dtype=numpy.float32, count=4)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_4f = struct.Struct("<4f")
