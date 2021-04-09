#!/usr/bin/env python

# Copyright 2013-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# -*- coding: utf-8
import Ice, sys, sha
from M2Crypto import X509;

Ice.loadSlice('', ['-I' + Ice.getSliceDir(), 'Murmur.ice'])
import Murmur

class MetaCallbackI(Murmur.MetaCallback):
    def started(self, s, current=None):
        print "started"
        serverR=Murmur.ServerCallbackPrx.uncheckedCast(adapter.addWithUUID(ServerCallbackI(server, current.adapter)))
        s.addCallback(serverR)

    def stopped(self, s, current=None):
        print "stopped"

class ServerCallbackI(Murmur.ServerCallback):
    def __init__(self, server, adapter):
      self.server = server
      self.contextR=Murmur.ServerContextCallbackPrx.uncheckedCast(adapter.addWithUUID(ServerContextCallbackI(server)))

    def userConnected(self, p, current=None):
      print "connected"
      print p
      self.server.addContextCallback(p.session, "flubber", "Power up the T", self.contextR, Murmur.ContextChannel | Murmur.ContextUser)
      if (self.server.hasPermission(p.session, 0, Murmur.PermissionWrite)):
        print "Is a global admin"
      certlist=self.server.getCertificateList(p.session)
      for cert in certlist:
         hash = sha.new()
         hash.update(cert)
         cert = X509.load_cert_der_string(cert)
         print cert.get_subject(), "issued by", cert.get_issuer(), "hash", hash.hexdigest()
      if current:
        print current.ctx

    def userDisconnected(self, p, current=None):
      print "disconnected"
      print p

    def userStateChanged(self, p, current=None):
      print "stateChanged"
      print self.server
      print p

    def userTextMessage(self, p, msg, current=None):
      print "userTextMessage"
      print self.server
      print p
      print msg

    def channelCreated(self, c, current=None):
      print "created"
      print c

    def channelRemoved(self, c, current=None):
      print "removed"
      print c

    def channelStateChanged(self, c, current=None):
      print "stateChanged"
      print c

class ServerContextCallbackI(Murmur.ServerContextCallback):
    def __init__(self, server):
      self.server = server

    def contextAction(self, action, p, session, chanid, current=None):
      print "aaand ACTION"
      print action
      print p
      print session
      print chanid
      if (session != 0):
        server.sendMessage(session, "Bouncy")
      elif (chanid >= 0):
        server.sendMessageChannel(chanid, 0, "Channel Bouncy")

if __name__ == "__main__":
    global contextR

    prop = Ice.createProperties(sys.argv)
    prop.setProperty("Ice.ImplicitContext", "Shared")

    idd = Ice.InitializationData()
    idd.properties = prop

    ice = Ice.initialize(idd)

    print "Creating callbacks...",

    # If icesecret is set, we need to set it here as well.
    ice.getImplicitContext().put("secret", "fourtytwo")

    meta = Murmur.MetaPrx.checkedCast(ice.stringToProxy('Meta:tcp -h 127.0.0.1 -p 6502'))
    adapter = ice.createObjectAdapterWithEndpoints("Callback.Client", "tcp -h 127.0.0.1")
    
    metaR=Murmur.MetaCallbackPrx.uncheckedCast(adapter.addWithUUID(MetaCallbackI()))
    adapter.activate()
    
    meta.addCallback(metaR)

    for server in meta.getBootedServers():
      serverR=Murmur.ServerCallbackPrx.uncheckedCast(adapter.addWithUUID(ServerCallbackI(server, adapter)))
      server.addCallback(serverR)

    print "Done"
    print 'Script running (press CTRL-C to abort)';
    try:
        ice.waitForShutdown()
    except KeyboardInterrupt:
        print 'CTRL-C caught, aborting'

    meta.removeCallback(metaR)
    ice.shutdown()
    print "Goodbye"
