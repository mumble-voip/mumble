#!/usr/bin/env python
# -*- coding: utf-8
import Ice, sys

Ice.loadSlice('Murmur.ice') 
import Murmur

class MetaCallbackI(Murmur.MetaCallback):
    def started(self, s, current=None):
        global serverR
        print "started"
        print s
        print s.id()
        s.addCallback(serverR)

    def stopped(self, s, current=None):
        print "stopped"
        print s
        print s.id()

class ServerCallbackI(Murmur.ServerCallback):
    def playerConnected(self, p, current=None):
      print "connected"
      print p

    def playerDisconnected(self, p, current=None):
      print "disconnected"
      print p

    def playerStateChanged(self, p, current=None):
      print "stateChanged"
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

if __name__ == "__main__":
    print "Creating callbacks...",
    ice = Ice.initialize(sys.argv)
    
    meta = Murmur.MetaPrx.checkedCast(ice.stringToProxy('Meta:tcp -h 127.0.0.1 -p 6502'))

    adapter = ice.createObjectAdapterWithEndpoints("Callback.Client", "tcp -h 127.0.0.1 -p 6503")

    adapter.add(MetaCallbackI(), ice.stringToIdentity("metaCallbackReceiver"))
    adapter.add(ServerCallbackI(), ice.stringToIdentity("serverCallbackReceiver"))
    adapter.activate()

    metaR = Murmur.MetaCallbackPrx.uncheckedCast(adapter.createProxy(ice.stringToIdentity("metaCallbackReceiver")))
    serverR = Murmur.ServerCallbackPrx.uncheckedCast(adapter.createProxy(ice.stringToIdentity("serverCallbackReceiver")))
    
    meta.addCallback(metaR)
    
    for server in meta.getBootedServers():
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