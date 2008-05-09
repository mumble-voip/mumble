#! /usr/bin/perl

use warnings;
use strict;

# If we're being run as a CGI in suexec, $HOME doesn't exist. Fake it.
my $home = (getpwuid($<))[7];

# This needs to be done before "use Net::DBus"
if (open(F, "$home/murmur/.dbus.sh")) {
  while(<F>) {
    chomp();
    if ($_ =~ /^(.+?)\='(.+)';$/) {
      $ENV{$1}=$2;
    }
  }
  close(F);
}

use Net::DBus;
use CGI;
use CGI::Carp qw(fatalsToBrowser);

my $q = new CGI;

print $q->header();
print $q->start_html(-title=>"Userlist");

my $bus;
my $service;

# First try the system bus
eval {
  $bus=Net::DBus->system();
  $service = $bus->get_service("net.sourceforge.mumble.murmur");
};

# If that failed, the session bus
if (! $service) {
  eval {
    $bus = Net::DBus->session();
    $service = $bus->get_service("net.sourceforge.mumble.murmur");
  }
}

die "Murmur service not found" if (! $service);

# Fetch handle to remote object
my $object = $service->get_object("/");
# Call a function on the murmur object
my $servers = $object->getBootedServers();

foreach my $server (@{$servers}) {
  my $name = $object->getConf($server, "registername");
  print "<h1>Server #$server $name</h1>\n";
  print "<table><tr><th>Name</th><th>Channel</th></tr>\n";
  my $servobj = $service->get_object("/$server");

  # First, get channel names
  my $channels = $servobj->getChannels();
  my %channels;
  foreach my $c (@{$channels}) {
    my @c = @{$c};
    my $id = $c[0];
    my $name = $c[1];
    $channels{$id}=$name;
  }

  # Then, get and print the players
  my $players = $servobj->getPlayers();
  foreach my $p (@{$players}) {
    my @p = @{$p};
    my $chanid = $p[6];
    my $name = $p[8];
    my $chan = $channels{$chanid};
    print "<tr><td>$name</td><td>$chan</td></tr>\n";
  }
  print "</table>\n";
}

print $q->end_html();
