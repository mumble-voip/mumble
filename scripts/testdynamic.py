#!/usr/bin/env python
# -*- coding: utf-8
import Ice, IcePy, sys, tempfile

ice = Ice.initialize(sys.argv)

proxy = ice.stringToProxy('Meta:tcp -h 127.0.0.1 -p 6502')
slice = IcePy.Operation('getSlice', Ice.OperationMode.Idempotent, Ice.OperationMode.Idempotent, True, (), (), (), IcePy._t_string, ()).invoke(proxy, ((), None))

slicefile = tempfile.NamedTemporaryFile(suffix = '.ice')
slicefile.write(slice)
slicefile.flush()
Ice.loadSlice(slicefile.name)
slicefile.close()

import Murmur

meta = Murmur.MetaPrx.checkedCast(proxy)

print meta.getVersion()
