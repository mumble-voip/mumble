// Copyright 2013-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/*
This shows how to list connected users in C#. This depends on the NDesk-DBus
package, which can be found at http://www.ndesk.org/DBusSharp

Note that DBusSharp requires you to define the interface of the object you
are connecting to at compile time. As such, you loose the advantages of DBus
Introspection, but it's a bit faster (and type safe).
This example only includes a very small subset of the interface definition.

To compile this with Mono, type:
gmcs -pkg:ndesk-dbus-1.0 ListUsers.cs

To compile this with the Microsoft compiler, type:
csc -r:NDesk.DBus.dll ListUsers.cs
*/

using System;
using NDesk.DBus;
using org.freedesktop.DBus;

public class ListUsers
{
	public static void Main (string[] args)
	{
		Connection bus;
		if (args.Length == 0) {
			bus = Bus.Session;
		} else {
			if (args[0] == "--session")
				bus = Bus.Session;
			else if (args[0] == "--system")
				bus = Bus.System;
			else
				bus = Connection.Open (args[0]);
		}

		Meta m = bus.GetObject<Meta>("net.sourceforge.mumble.murmur", new ObjectPath("/"));
		int [] servers = m.getBootedServers();

		foreach(int id in servers) {
			string path="/"+id;
			Console.WriteLine("Users on server #{0}, path {1}",id,path);
			Server s = bus.GetObject<Server>("net.sourceforge.mumble.murmur", new ObjectPath(path));
			PlayerInfoExtended [] players = s.getPlayers();
			foreach(PlayerInfoExtended pi in players) {
				Console.WriteLine("  {0} ({1})",pi.name,pi.session);
			}
		}
	}
}

[Interface ("net.sourceforge.mumble.Meta")]
public interface Meta : Introspectable
{
	int [] getBootedServers();
}

[Interface ("net.sourceforge.mumble.Murmur")]
public interface Server : Introspectable
{
	PlayerInfoExtended [] getPlayers();
}

public struct PlayerInfoExtended
{
	public uint session;
	public bool mute;
	public bool deaf;
	public bool suppressed;
	public bool selfMute;
	public bool selfDeaf;
	public int channel;
	public int id;
	public string name;
	public int onlinesecs;
	public int bytespersec;
}
