#! /usr/bin/perl 

# This is a small example script of how to set up murmur to authenticate through
# phpBB3. To use it, you'll have to have started murmur with DBus, and use the
# same session for this script.
#

use warnings;
use strict;

# Replace these with whatever is correct for you
our $dbname="phpbb3";
our $dbuser="phpbb3";
our $dbpass="uhduqw1237a";
our $dbprefix="phpbb_";
our $dbhost="localhost";

# Assign user id as phpbb3 user_id plus this, to avoid clashing
# with local murmur users. If you're going to use ONLY external 
# authentication, you can set this to 1, but there's no real point.
our $id_offset = 100000;

# Path to phpBB user avatars. If you want to disable avatar support, set
# this blank. This can be either a directory path or a full URL.

our $avatar_path = "http://xeno.stud.hive.no/phpBB3/download.php?avatar=";

# No user servicable parts below this point.
use DBI;
use Net::DBus;
use Data::Dumper;
use Net::DBus::Reactor;
use LWP::UserAgent;

our %texturecache;

our @dbhparams=("dbi:mysql:dbname=${dbname};host=${dbhost}", $dbuser, $dbpass);
our $agent=new LWP::UserAgent;
$agent->timeout(5);

our $r = Net::DBus::Reactor->main;
our $bus = Net::DBus->session();

our $service = $bus->get_service("net.sourceforge.mumble.murmur");
our $object = $service->get_object("/");

our $rservice = $bus->export_service("net.sourceforge.mumble.phpbb");
our $robject = Mumble::Auth->new($rservice);

my $response = $object->setAuthenticator("/authority");

package Mumble::Auth;

use Data::Dumper;
use Image::Magick;
use Digest::MD5 qw(md5_hex);
use Net::DBus::Exporter qw(net.sourceforge.mumble.auther);
use base qw(Net::DBus::Object);
dbus_method("authenticate", ["string","string"], ["int32"]);
dbus_method("getUserName", ["int32"], ["string"]);
dbus_method("getUserId", ["string"], ["int32"]);
dbus_method("getUserTexture", ["int32"], [["array", "byte"]]);

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
# -1 Wrong password
# -2 Unknown user -- fall back to builtin database

sub authenticate {
  my $self = shift;
  my $uname = shift;
  my $pw = shift;
  
  my $dbh=DBI->connect_cached(@dbhparams);
  if (! $dbh) {
    carp $DBI::errstr;
    return -2;
  }
  my $sth=$dbh->prepare("SELECT user_id, user_password, user_type FROM ${dbprefix}users WHERE username = ?");
  $sth->execute($uname);
  if ((my $r=$sth->fetchrow_hashref())) {
    if ($$r{'user_password'} ne md5_hex($pw)) {
      print "Wrong password for $uname\n";
      return -1;
    }
    if (($$r{'user_type'} != 0) && ($$r{'user_type'} != 3)) {
      return -1;
    }
    my $id = $$r{'user_id'} + $id_offset;
    $sth->finish();

    my @groups;
    $sth=$dbh->prepare("SELECT group_name FROM ${dbprefix}user_group LEFT JOIN ${dbprefix}groups USING (group_id) WHERE user_id = ?");
    $sth->execute($$r{'user_id'});
    while ((my $g=$sth->fetchrow_hashref())) {
      push @groups, lc $$g{'group_name'};
    }
    my $response = $object->setTemporaryGroups(0, $id, \@groups);
    Dumper($response);
    print "Authenticated $uname as ID $id with $#groups groups\n";
    return $id;
  } else {
    print "Unknown user $uname\n";
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

  my $dbh=DBI->connect_cached(@dbhparams);
  if (! $dbh) {
    carp $DBI::errstr;
    return undef;
  }
  my $sth=$dbh->prepare("SELECT username FROM ${dbprefix}users WHERE user_id = ?");
  $sth->execute($id - $id_offset);
  if ((my $r=$sth->fetchrow_hashref())) {
    return $$r{'username'};
  }
  return undef;
}

# Same response as authenticate

sub getUserId {
  my $self = shift;
  my $name = shift;

  my $dbh=DBI->connect_cached(@dbhparams);
  if (! $dbh) {
    carp $DBI::errstr;
    return -2;
  }
  my $sth=$dbh->prepare("SELECT user_id FROM ${dbprefix}users WHERE username = ?");
  $sth->execute($name);
  if ((my $r=$sth->fetchrow_hashref())) {
    return $$r{'user_id'} + $id_offset;
  }
  return -2;
}

# Grab a user texture.

sub getUserTexture {
  my $self = shift;
  my $uid = shift;

  my @a;

  my $dbh=DBI->connect_cached(@dbhparams);
  if (! $dbh) {
    carp $DBI::errstr;
    return undef;
  }
  my $sth=$dbh->prepare("SELECT user_avatar, user_avatar_type FROM ${dbprefix}users WHERE user_id = ?");
  $sth->execute($uid - $id_offset);
  if ((my $r=$sth->fetchrow_hashref())) {
    my $file = $$r{'user_avatar'};
    my $type = $$r{'user_avatar_type'};
    if ($type != 1) {
      print "Request for texture $uid :: not uploaded texture\n";
      return \@a;
    }
    if (exists $texturecache{$file}) {
      return $texturecache{$file};
    }
    
    my $response = $agent->get($avatar_path . $file);
    if (! $response->is_success) {
      print "Request for texture $uid :: Fetch failed: ". $response->status_line . "\n";
    } else {
      my $image = new Image::Magick();
      my $r = $image->BlobToImage($response->content);
      if ($r) {
        print "Request for texture $uid :: Image load failed: $r\n";
      } else {
        $image->Extent(x => 0, y => 0, width => 600, height => 60);
        my $out=$image->ImageToBlob(magick => 'rgba', depth => 8);
        if (length($out) != (600*60*4)) {
          print "Request for texture $uid :: Failed resize\n";
        } else {
          @a = unpack("C*", $out);
          for(my $i=0;$i<600*60;$i++) {
            my $red=$a[$i*4];
            my $blue=$a[$i*4+2];
            $a[$i*4]=$blue;
            $a[$i*4+2]=$red;
          }
          print "Request for texture $uid :: Success\n";
        }
      }
    }
    $texturecache{$file} = \@a;
    return $texturecache{$file};
  }
  return undef;
}


package main;

$r->run();
