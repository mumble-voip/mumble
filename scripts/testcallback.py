#!/usr/bin/env python
# -*- coding: utf-8
import Ice, sys

Ice.loadSlice('Murmur.ice') 
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

    def playerConnected(self, p, current=None):
      print "connected"
      print p
      self.server.addContextCallback(p.session, "flubber", "Power up the T", self.contextR, Murmur.ContextChannel | Murmur.ContextPlayer)

    def playerDisconnected(self, p, current=None):
      print "disconnected"
      print p

    def playerStateChanged(self, p, current=None):
      print "stateChanged"
      print self.server
      print p

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

    print "Creating callbacks...",
    ice = Ice.initialize(sys.argv)
    
    meta = Murmur.MetaPrx.checkedCast(ice.stringToProxy('Meta:tcp -h 127.0.0.1 -p 49152'))

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
