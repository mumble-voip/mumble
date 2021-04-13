#!/usr/bin/env ruby

# Copyright 2013-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.


# Download Murmur.ice from
# http://mumble.git.sourceforge.net/git/gitweb.cgi?p=mumble;a=blob_plain;f=src/murmur/Murmur.ice;hb=HEAD
# and run 'slice2rb Murmur.ice'. slice2rb is part of ICE.
# This will generate the necessary 'Murmur.rb' file which needs to be included:

require 'Murmur.rb'

status = 0
ic = nil
begin
  ic = Ice::initialize(ARGV)
  base = ic.stringToProxy("Meta:tcp -h 127.0.0.1 -p 6502")
  meta = Murmur::MetaPrx::checkedCast(base)

  unless meta
    rais "invalid proxy"
  end

  # All commands are documented here:
  # http://mumble.sourceforge.net/slice/?title=slice

  servers = meta.getBootedServers
  puts "Server version:", meta.getVersion[3]

  servers.each do |server|
    puts ""
    puts "Server name:"+server.getConf('registername')

    channels = server.getChannels
    players = server.getPlayers

    players.each do |id,player|
      channel = channels[player.channel]
      puts player.name+" in "+channel.name
    end

  end

rescue
  puts $!
  puts $!.backtrace.join("\n")
  status = 1
end

if ic
  # Clean up
  begin
    ic.destroy()
  rescue
    puts $!
    puts $!.backtrace.join("\n")
    status = 1
  end
end

exit(status)
