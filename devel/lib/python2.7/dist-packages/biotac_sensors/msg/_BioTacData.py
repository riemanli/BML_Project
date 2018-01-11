"""autogenerated by genpy from biotac_sensors/BioTacData.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class BioTacData(genpy.Message):
  _md5sum = "23cbcbcbf4470cc3c87ecb98037f4bb0"
  _type = "biotac_sensors/BioTacData"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """string bt_serial
uint16 bt_position
uint16 tdc_data
uint16 tac_data
uint16 pdc_data
uint16[22] pac_data
uint16[19] electrode_data

"""
  __slots__ = ['bt_serial','bt_position','tdc_data','tac_data','pdc_data','pac_data','electrode_data']
  _slot_types = ['string','uint16','uint16','uint16','uint16','uint16[22]','uint16[19]']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       bt_serial,bt_position,tdc_data,tac_data,pdc_data,pac_data,electrode_data

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(BioTacData, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.bt_serial is None:
        self.bt_serial = ''
      if self.bt_position is None:
        self.bt_position = 0
      if self.tdc_data is None:
        self.tdc_data = 0
      if self.tac_data is None:
        self.tac_data = 0
      if self.pdc_data is None:
        self.pdc_data = 0
      if self.pac_data is None:
        self.pac_data = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
      if self.electrode_data is None:
        self.electrode_data = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
    else:
      self.bt_serial = ''
      self.bt_position = 0
      self.tdc_data = 0
      self.tac_data = 0
      self.pdc_data = 0
      self.pac_data = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
      self.electrode_data = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]

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
      _x = self.bt_serial
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      _x = self
      buff.write(_struct_4H.pack(_x.bt_position, _x.tdc_data, _x.tac_data, _x.pdc_data))
      buff.write(_struct_22H.pack(*self.pac_data))
      buff.write(_struct_19H.pack(*self.electrode_data))
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
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.bt_serial = str[start:end].decode('utf-8')
      else:
        self.bt_serial = str[start:end]
      _x = self
      start = end
      end += 8
      (_x.bt_position, _x.tdc_data, _x.tac_data, _x.pdc_data,) = _struct_4H.unpack(str[start:end])
      start = end
      end += 44
      self.pac_data = _struct_22H.unpack(str[start:end])
      start = end
      end += 38
      self.electrode_data = _struct_19H.unpack(str[start:end])
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
      _x = self.bt_serial
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      _x = self
      buff.write(_struct_4H.pack(_x.bt_position, _x.tdc_data, _x.tac_data, _x.pdc_data))
      buff.write(self.pac_data.tostring())
      buff.write(self.electrode_data.tostring())
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
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.bt_serial = str[start:end].decode('utf-8')
      else:
        self.bt_serial = str[start:end]
      _x = self
      start = end
      end += 8
      (_x.bt_position, _x.tdc_data, _x.tac_data, _x.pdc_data,) = _struct_4H.unpack(str[start:end])
      start = end
      end += 44
      self.pac_data = numpy.frombuffer(str[start:end], dtype=numpy.uint16, count=22)
      start = end
      end += 38
      self.electrode_data = numpy.frombuffer(str[start:end], dtype=numpy.uint16, count=19)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_19H = struct.Struct("<19H")
_struct_4H = struct.Struct("<4H")
_struct_22H = struct.Struct("<22H")
