#!/usr/bin/env python

import sys, Ice

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

class Client(Ice.Application):
    def run(self, args):
        global serverR

        meta = Murmur.MetaPrx.checkedCast(self.communicator().stringToProxy('Meta:tcp -h 127.0.0.1 -p 49152'))

        adapter = self.communicator().createObjectAdapterWithEndpoints("Callback.Client", "tcp -h 127.0.0.1 -p 49153")

        adapter.add(MetaCallbackI(), self.communicator().stringToIdentity("metaCallbackReceiver"))
        adapter.add(ServerCallbackI(), self.communicator().stringToIdentity("serverCallbackReceiver"))
        adapter.activate()

        metaR = Murmur.MetaCallbackPrx.uncheckedCast(adapter.createProxy(self.communicator().stringToIdentity("metaCallbackReceiver")))
        serverR = Murmur.ServerCallbackPrx.uncheckedCast(adapter.createProxy(self.communicator().stringToIdentity("serverCallbackReceiver")))

        meta.addCallback(metaR)
        
        for server in meta.getBootedServers():
          server.addCallback(serverR)
        
        print 'Ready to go';

        c = None
        while c != 'x':
            try:
                c = raw_input("==> ")
                if c == 'x':
                    pass # Nothing to do
            except EOFError:
                break
            except KeyboardInterrupt:
                break

        meta.removeCallback(metaR)

        return 0

app = Client()
sys.exit(app.main(sys.argv))
