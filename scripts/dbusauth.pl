#! /usr/bin/perl 

package Mumble::Auth;

use Net::DBus::Exporter qw(net.sourceforge.mumble.auther);
use base qw(Net::DBus::Object);
dbus_method("authenticate", ["string","string"], ["int32"]);
dbus_method("getUserName", ["int32"], ["string"]);
dbus_method("getUserId", ["string"], ["int32"]);

sub new {
  my $class = shift;
  my $service = shift;
  my $self = $class->SUPER::new($service, "/authority");
  bless $self, $class;
  return $self;
}

# Possible responses are:
# >0 ID of user
# 0 SuperUser
# -1 Unknown user
# -2 Unknown user -- fall back to builtin database

sub authenticate {
  my $self = shift;
  my $uname = shift;
  my $pw = shift;
  
  if (($uname eq "SuperUser") && ($pw eq "blipp")) {
    return 0;
  } else {
    return -2;
  }
}

# Possible responses are:
# string Name of user
# empty  Unknown user
# undef  Fall back to builting database

sub getUserName {
  my $self = shift;
  my $id = shift;
  if ($id == 0) {
    return "SupahUsah";
  }
  return undef;
}

# Same response as authenticate

sub getUserId {
  my $self = shift;
  my $name = shift;
  if ($name eq "SupahUsah") {
    return 0;
  } else {
    return -2;
  }
}

package main;

use strict;
use warnings;
use Net::DBus;
use Data::Dumper;
use Net::DBus::Reactor;

my $r = Net::DBus::Reactor->main;
my $bus = Net::DBus->session();

my $service = $bus->get_service("net.sourceforge.mumble");
my $object = $service->get_object("/Murmur");

my $response = $object->getPlayers();

print "R: ";
print Dumper($response);

my $rservice = $bus->export_service("net.sourceforge.mumble.test");
my $robject = Mumble::Auth->new($rservice);

my $response = $object->setAuthenticator("/authority");

$r->run();

