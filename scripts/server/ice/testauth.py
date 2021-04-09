#!/usr/bin/env python

# Copyright 2013-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# -*- coding: utf-8
import Ice, sys, time
from M2Crypto import X509;
Ice.loadSlice('', ['-I' + Ice.getSliceDir(), 'Murmur.ice'])
import Murmur

class ServerAuthenticatorI(Murmur.ServerUpdatingAuthenticator):
    def __init__(self, server, adapter):
      self.server = server

    def authenticate(self, name, pw, certlist, certhash, strong, current=None):
      print certhash, strong
      for cert in certlist:
        cert = X509.load_cert_der_string(cert)
        print cert.get_subject(), "issued by", cert.get_issuer()
      groups = ("GroupA", "GroupB");
      if (name == "One"):
        if (pw == "Magic"):
          return (1, "One", groups)
        else:
          return (-1, None, None)
      elif (name == "Two"):
        if (pw == "Mushroom"):
          return (2, "twO", groups)
        else:
          return (-1, None, None)
      elif (name == "White Space"):
        if (pw == "Space White"):
          return (3, "White Space", groups)
        else:
          return (-1, None, None)
      elif (name == "Fail"):
        time.sleep(6)
      return (-2, None, None)

    def getInfo(self, id, current=None):
      print "getInfo ", id
      name = self.idToName(id);
      if (name == None):
        return (False, {})
      map = {}
      map[Murmur.UserInfo.UserName]=name
      return (True, map)

    def nameToId(self, name, current=None):
      if (name == "One"):
        return 1
      elif (name == "Twoer"):
        return 2
      else:
        return -2;

    def idToName(self, id, current=None):
      if (id == 1):
        return "One"
      elif (id == 2):
        return "Two"
      else:
        return None

    def idToTexture(self, id, current=None):
      print "idToTexture", id
      return open("../icons/mumble.osx.png").read();

    # The expanded methods from UpdatingAuthenticator. We only implement a subset for this example, but
    # a valid implementation has to define all of them
    def registerUser(self, name, current=None):
      print "Someone tried to register " + name[Murmur.UserInfo.UserName]
      return -2

    def unregisterUser(self, id, current=None):
      print "Unregister ", id
      return -2

    def getRegistration(self, id, current=None):
      return (-2, None, None)
    
    def setInfo(self, id, info, current=None):
      print "Set", id, info
      return -1

if __name__ == "__main__":
    global contextR

    print "Creating callbacks...",
    ice = Ice.initialize(sys.argv)

    meta = Murmur.MetaPrx.checkedCast(ice.stringToProxy('Meta:tcp -h 127.0.0.1 -p 6502'))

    adapter = ice.createObjectAdapterWithEndpoints("Callback.Client", "tcp -h 127.0.0.1")
    adapter.activate()

    for server in meta.getBootedServers():
      serverR=Murmur.ServerUpdatingAuthenticatorPrx.uncheckedCast(adapter.addWithUUID(ServerAuthenticatorI(server, adapter)))
      server.setAuthenticator(serverR)

    print "Done"
    
    map = {};
    map[Murmur.UserInfo.UserName] = 'TestUser';

    for server in meta.getBootedServers():
      ids= server.getUserIds(["TestUser"])
      for name,id in ids.iteritems():
        if (id > 0):
          print "Will unregister ", id
          server.unregisterUser(id)
      server.registerUser(map)

    print 'Script running (press CTRL-C to abort)';
    try:
        ice.waitForShutdown()
    except KeyboardInterrupt:
        print 'CTRL-C caught, aborting'

    meta.removeCallback(metaR)
    ice.shutdown()
    print "Goodbye"
